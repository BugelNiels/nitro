#include "renderview.hpp"

#include <math.h>

#include <QLoggingCategory>
#include <QMouseEvent>
#include <QOpenGLVersionFunctionsFactory>

nitro::RenderView::RenderView(QWidget *Parent) : QOpenGLWidget(Parent), shiftPressed(false) { resetOrientation(); }

nitro::RenderView::~RenderView() { makeCurrent(); }

void nitro::RenderView::initializeGL() {
    initializeOpenGLFunctions();

    QString glVersion;
    glVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    makeCurrent();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // grab the opengl context
    QOpenGLFunctions_4_1_Core *functions =
            QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_1_Core>(this->context());

    // initialize renderers here with the current context
    renderer.init(functions, &settings);

    updateMatrices();
}

void nitro::RenderView::resizeGL(int newWidth, int newHeight) {
    settings.dispRatio = float(newWidth) / float(newHeight);

    settings.projectionMatrix.setToIdentity();
    settings.projectionMatrix.perspective(settings.fov, settings.dispRatio, 0.5f, 10.0f);
    updateMatrices();
}

void nitro::RenderView::updateBuffers(const QImage &image) { renderer.updateBuffers(image); }

void nitro::RenderView::updateProjectionMatrix() {
    settings.projectionMatrix.setToIdentity();
    settings.projectionMatrix.perspective(settings.fov, settings.dispRatio, 0.5f, 10.0f);
}

void nitro::RenderView::resetModelViewMatrix() {
    settings.modelViewMatrix.setToIdentity();
    settings.modelViewMatrix.translate(translation);
}

void nitro::RenderView::resetOrientation() {
    scale = 1.0f;
    translation = QVector3D(0.0, 0.0, -settings.distFromCamera);
    rotationQuaternion = QQuaternion();
}

void nitro::RenderView::updateMatrices() {
    resetModelViewMatrix();
    settings.modelViewMatrix.scale(scale);
    settings.modelViewMatrix.rotate(rotationQuaternion);

    settings.normalMatrix = settings.modelViewMatrix.normalMatrix();
    // create the matrix to map clipping space coordinates to world space
    // coordinates. Do this calculation here to prevent redoing this calculating
    // everytime the mouse is pressed. Only needs to be updated whenever the
    // projection or the modelview matrix is being updated
    bool inverted = false;
    settings.toWorldCoordsMatrix = (settings.modelViewMatrix).inverted(&inverted);
//    settings.toWorldCoordsMatrix = (settings.projectionMatrix * settings.modelViewMatrix).inverted(&inverted);

    settings.uniformUpdateRequired = true;

    update();
}

void nitro::RenderView::paintGL() {
    QVector3D bCol = {1, 1, 1};
    glClearColor(bCol.x(), bCol.y(), bCol.z(), 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    renderer.draw();
}

/**
 * @brief MainView::toNormalizedScreenCoordinates Normalizes the mouse
 * coordinates to screen coordinates.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return A vector containing the normalized x and y screen coordinates.
 */
QVector2D nitro::RenderView::toNormalizedScreenCoordinates(float x, float y) const {
    float xRatio = x / float(width());
    float yRatio = y / float(height());

    // By default, the drawing canvas is the square [-1,1]^2:
    float xScene = (1 - xRatio) * -1 + xRatio * 1;
    // Note that the origin of the canvas is in the top left corner (not the lower
    // left).
    float yScene = yRatio * -1 + (1 - yRatio) * 1;

    return {xScene, yScene};
}

/**
 * @brief RenderView::mouseMoveRotate Updates the camera rotation based on mouse
 * movement.
 * @param Event Mouse event.
 */
void nitro::RenderView::mouseMoveRotate(QMouseEvent *event) {
    QVector2D sPos = toNormalizedScreenCoordinates(event->position().x(), event->position().y());
    QVector3D newVec = QVector3D(sPos.x(), sPos.y(), 0.0);

    // project onto sphere
    float sqrZ = 1.0f - QVector3D::dotProduct(newVec, newVec);
    if (sqrZ > 0) {
        newVec.setZ(sqrt(sqrZ));
    } else {
        newVec.normalize();
    }

    QVector3D v2 = newVec.normalized();
    // reset if we are starting a drag
    if (!dragging) {
        dragging = true;
        oldRotationVec = v2;
        return;
    }

    // calculate axis and angle
    QVector3D v1 = oldRotationVec;
    QVector3D N = QVector3D::crossProduct(v1, v2).normalized();
    if (N.length() == 0.0f) {
        oldRotationVec = v2;
        return;
    }
    float angle = 180.0f / M_PI * acos(QVector3D::dotProduct(v1, v2));
    rotationQuaternion = QQuaternion::fromAxisAndAngle(N, angle) * rotationQuaternion;
    updateMatrices();

    // for next iteration
    oldRotationVec = v2;
}

/**
 * @brief RenderView::mouseMoveTranslate Updates camera translation based on
 * mouse movement.
 * @param event Mouse event.
 */
void nitro::RenderView::mouseMoveTranslate(QMouseEvent *event) {
    QVector2D sPos = toNormalizedScreenCoordinates(event->position().x(), event->position().y());
    if (!dragging) {
        dragging = true;
        oldMouseCoords = sPos;
        return;
    }
    QVector3D translationUpdate = QVector3D(sPos.x() - oldMouseCoords.x(), sPos.y() - oldMouseCoords.y(), 0.0);
    translationUpdate *= settings.dragSensitivity;
    translation += translationUpdate;
    updateMatrices();
    oldMouseCoords = sPos;
}

/**
 * @brief MeshView::mouseMoveEvent Event that is called when the mouse is moved.
 * @param Event The mouse event.
 */
void nitro::RenderView::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton) {
        if (shiftPressed) {
            mouseMoveTranslate(event);
        } else {
            mouseMoveRotate(event);
        }
    } else {
        dragging = false;
    }
}

/**
 * @brief RenderView::mousePressEvent Event that is called when the mouse is
 * pressed.
 * @param event The mouse event.
 */
void nitro::RenderView::mousePressEvent(QMouseEvent *event) {
    QOpenGLWidget::mousePressEvent(event);
    setFocus();
}


/**
 * @brief RenderView::mousePressEvent Event that is called when the mouse is
 * pressed.
 * @param event The mouse event.
 */
void nitro::RenderView::mouseReleaseEvent(QMouseEvent *event) {
    QOpenGLWidget::mouseReleaseEvent(event);
    dragging = false;
    setFocus(); }

/**
 * @brief MainView::wheelEvent Event that is called when the user scrolls.
 * @param event The mouse event.
 */
void nitro::RenderView::wheelEvent(QWheelEvent *event) {
    float phi = 1.0f + (event->angleDelta().y() / 2000.0f);
    scale = fmin(fmax(phi * scale, 0.01f), 100.0f);
    updateMatrices();
}

/**
 * @brief MainView::keyPressEvent Event that is called when a key is pressed.
 * @param event The key event.
 */
void nitro::RenderView::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case 'R':
            resetOrientation();
            updateMatrices();
            update();
            break;
        case Qt::Key_Shift:
            shiftPressed = true;
    }
}

/**
 * @brief MainView::keyReleaseEvent Event that is called when a key is released.
 * @param event The key event.
 */
void nitro::RenderView::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Shift:
            shiftPressed = false;
    }
}