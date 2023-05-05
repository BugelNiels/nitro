#include "renderview.hpp"

#include <math.h>

#include <QLoggingCategory>
#include <QMouseEvent>
#include <QOpenGLVersionFunctionsFactory>

nitro::RenderView::RenderView(QWidget *Parent) : QOpenGLWidget(Parent) {
    setMouseTracking(true);
    resetOrientation();
    setFocusPolicy(Qt::StrongFocus);
}

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
    auto *functions = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_1_Core>(this->context());

    // initialize renderers here with the current context
    renderer.init(functions, &settings);

    updateMatrices();
    int w = 256;
    int h = 256;
    QImage img(w, h, QImage::Format_Grayscale8);
    for (int y = 0; y < h; y++) {
        auto *row = img.scanLine(y);
        for (int x = 0; x < w; x++) {
            row[x] = x;
        }
    }
    resetOrientation();
    updateBuffers(img);
}

void nitro::RenderView::resizeGL(int newWidth, int newHeight) {
    widgetMidPoint = mapToGlobal(QPoint(width() / 2, height() / 2));
    settings.dispRatio = float(newWidth) / float(newHeight);

    settings.projectionMatrix.setToIdentity();
    settings.projectionMatrix.perspective(settings.fov, settings.dispRatio, 0.5f, 10.0f);
    updateMatrices();
}

void nitro::RenderView::updateBuffers(const QImage &image) {
    renderer.updateBuffers(image);
    repaint();
}

void nitro::RenderView::resetModelViewMatrix() {
    settings.cameraMatrix.setToIdentity();
}

void nitro::RenderView::resetOrientation() {
    resetModelViewMatrix();
    settings.uniformUpdateRequired = true;
    translation = QVector3D(0.0, 0.0, settings.distFromCamera);
    camPitch = 0;
    camYaw = 0;
}

void nitro::RenderView::updateMatrices() {
    settings.cameraMatrix.setToIdentity();
    settings.cameraMatrix.translate(translation);
    settings.cameraMatrix.rotate(QQuaternion::fromEulerAngles(camPitch, camYaw, 0));

    settings.uniformUpdateRequired = true;

    repaint();
}

void nitro::RenderView::paintGL() {
    QVector3D bCol = {1, 1, 1};
    glClearColor(bCol.x(), bCol.y(), bCol.z(), 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (firstPersonMode) {
        QPointF diff = QCursor::pos() - widgetMidPoint;
        float sensitivity = 0.4f;
        diff *= sensitivity;
        camYaw -= diff.x();
        camPitch -= diff.y();
        camPitch = std::clamp(camPitch, -45.0f, 45.0f);
        QCursor::setPos(widgetMidPoint);
        QMatrix4x4 rot;
        rot.rotate(QQuaternion::fromEulerAngles(camPitch, camYaw, 0));

        QVector4D movDif = QVector4D(0, 0, 0, 0);
        if (keys[Qt::Key_W]) {
            movDif.setZ(movementSpeed);
        }
        if (keys[Qt::Key_A]) {
            movDif.setX(movementSpeed);
        }
        if (keys[Qt::Key_S]) {
            movDif.setZ(movDif.z() - movementSpeed);
        }
        if (keys[Qt::Key_D]) {
            movDif.setX(movDif.x() - movementSpeed);
        }
        if (keys[Qt::Key_Q]) {
            movDif.setY(movementSpeed);
        }
        if (keys[Qt::Key_E]) {
            movDif.setY(movDif.y() - movementSpeed);
        }
        translation -= (rot * movDif).toVector3D();

        settings.cameraMatrix.setToIdentity();
        settings.cameraMatrix.translate(translation);
        settings.cameraMatrix *= rot;
        settings.uniformUpdateRequired = true;
    }

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

    QVector3D v1 = oldRotationVec;
    QVector3D v2 = newVec.normalized();
    oldRotationVec = v2;
    // reset if we are starting a drag
    if (!dragging) {
        dragging = true;
        oldTranslation = translation;
        return;
    }

    // calculate axis and angle
    QVector3D N = QVector3D::crossProduct(v1, v2).normalized();
    if (N.length() == 0.0f) {
        return;
    }
    float angle = 180.0f / M_PI * acos(QVector3D::dotProduct(v1, v2));
    auto quat = QQuaternion::fromAxisAndAngle(N, -angle);
    QMatrix4x4 rot;
    rot.rotate(quat);
    translation = (rot * QVector4D(oldTranslation, 0)).toVector3D();
    auto angles = (QQuaternion::fromEulerAngles(camPitch, camYaw, 0) * quat).toEulerAngles();
    camPitch = angles.x();
    camYaw = angles.y();
    updateMatrices();
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


    QMatrix4x4 rot;
    rot.rotate(QQuaternion::fromEulerAngles(camPitch, camYaw, 0));
    QVector4D movDif = QVector4D(translationUpdate, 0);
    translation -= (rot * movDif).toVector3D();
    updateMatrices();
    oldMouseCoords = sPos;
}

/**
 * @brief MeshView::mouseMoveEvent Event that is called when the mouse is moved.
 * @param Event The mouse event.
 */
void nitro::RenderView::mouseMoveEvent(QMouseEvent *event) {

    if (firstPersonMode) {
        return;
    }
    if (event->buttons() == Qt::LeftButton) {
        setCursor(Qt::ClosedHandCursor);
        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            mouseMoveTranslate(event);
        } else {
            mouseMoveRotate(event);
        }
    } else {
        setCursor(Qt::OpenHandCursor);
    }
}

/**
 * @brief RenderView::mousePressEvent Event that is called when the mouse is
 * pressed.
 * @param event The mouse event.
 */
void nitro::RenderView::mousePressEvent(QMouseEvent *event) {
    QOpenGLWidget::mousePressEvent(event);
    disableFirstPerson();
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
    setFocus();
}

/**
 * @brief MainView::wheelEvent Event that is called when the user scrolls.
 * @param event The mouse event.
 */
void nitro::RenderView::wheelEvent(QWheelEvent *event) {
    float zoomFactor = 15.0;
    float zoom = event->angleDelta().y() > 0 ? zoomFactor : -zoomFactor;

    QMatrix4x4 rot;
    rot.rotate(QQuaternion::fromEulerAngles(camPitch, camYaw, 0));
    QVector4D movDif = QVector4D(0, 0, zoom, 0);
    translation -= (rot * movDif).toVector3D();

    updateMatrices();
}

/**
 * @brief MainView::keyPressEvent Event that is called when a key is pressed.
 * @param event The key event.
 */
void nitro::RenderView::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers().testFlag(Qt::ShiftModifier)) {
        if (event->key() == Qt::Key_F) {
            toggleFirstPerson();
            return;
        }
        movementSpeed = 4;
    }
    keys[event->key()] = true;
    switch (event->key()) {
        case Qt::Key_Escape:
            disableFirstPerson();
            break;
        case Qt::Key_R:
            disableFirstPerson();
            resetOrientation();
            break;
    }
    if (!firstPersonMode) {
        updateMatrices();
    }
}


/**
 * @brief MainView::keyReleaseEvent Event that is called when a key is released.
 * @param event The key event.
 */
void nitro::RenderView::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Shift) {
        qDebug() << "Triggered";
        movementSpeed = 1;
    }
    keys[event->key()] = false;
}

void nitro::RenderView::toggleFirstPerson() {
    if (firstPersonMode) {
        disableFirstPerson();
    } else {
        enableFirstPerson();
    }
}

void nitro::RenderView::disableFirstPerson() {
    if (frameTimer != -1) {
        killTimer(frameTimer);
        frameTimer = -1;
    }
    firstPersonMode = false;
    setCursor(Qt::OpenHandCursor);

}

void nitro::RenderView::enableFirstPerson() {
    setCursor(Qt::BlankCursor);
    QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
    oldMouseCoords = toNormalizedScreenCoordinates(width() / 2.0f, height() / 2.0f);
    frameTimer = startTimer(1000 / 60);
    firstPersonMode = true;
}


void nitro::RenderView::timerEvent(QTimerEvent *event) {
    repaint();
}
