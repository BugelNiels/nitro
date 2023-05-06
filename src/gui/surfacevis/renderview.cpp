#include "renderview.hpp"

#include <math.h>

#include <QLoggingCategory>
#include <QMouseEvent>
#include <QOpenGLVersionFunctionsFactory>
#include <QMenu>
#include <QPainter>
#include <QElapsedTimer>
#include <QPainterPath>

nitro::RenderView::RenderView(QWidget *Parent) : QOpenGLWidget(Parent) {
    setMouseTracking(true);
    resetOrientation();
    setFocusPolicy(Qt::StrongFocus);

}

nitro::RenderView::~RenderView() { makeCurrent(); }

void nitro::RenderView::initializeGL() {
    initializeOpenGLFunctions();

    connect(&debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this,
            SLOT(onMessageLogged(QOpenGLDebugMessage)), Qt::DirectConnection);

    if (debugLogger.initialize()) {
        QLoggingCategory::setFilterRules(
                "qt.*=false\n"
                "qt.text.font.*=false");

        debugLogger.startLogging(QOpenGLDebugLogger::SynchronousLogging);
        debugLogger.enableMessages();
    }

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

    resetOrientation();
//    int w = 256;
//    int h = 256;
//    QImage img(w, h, QImage::Format_Grayscale8);
//    for (int y = 0; y < h; y++) {
//        auto *row = img.scanLine(y);
//        for (int x = 0; x < w; x++) {
//            row[x] = x;
//        }
//    }
//    updateBuffers(img);
}


void nitro::RenderView::resizeGL(int newWidth, int newHeight) {
    widgetMidPoint = mapToGlobal(QPoint(width() / 2, height() / 2));


    int crossHairSize = 15;
    crossHair.clear();
    crossHair.moveTo(width() / 2, height() / 2 + crossHairSize);
    crossHair.lineTo(width() / 2, height() / 2 - crossHairSize);
    crossHair.moveTo(width() / 2 + crossHairSize, height() / 2);
    crossHair.lineTo(width() / 2 - crossHairSize, height() / 2);

    settings.dispRatio = float(newWidth) / float(newHeight);

    settings.projectionMatrix.setToIdentity();
    settings.projectionMatrix.perspective(settings.fov, settings.dispRatio, 0.5f, 10.0f);
    updateMatrices();
}

void nitro::RenderView::updateBuffers(const QImage &image) {
    renderer.updateBuffers(image);
    update();
}

void nitro::RenderView::resetOrientation() {
    initialCamPos = QVector3D(0.0, 0.0, settings.distFromCamera);
    initialCamRotVec = {1, 0, 0};
    initialCamAngle = 90;
    settings.cameraMatrix.setToIdentity();
    settings.uniformUpdateRequired = true;
    translation = {0, 0, 0};
    camPitch = 0;
    camYaw = 0;
}

void nitro::RenderView::updateMatrices() {
    recalcCamMatrix();
    update();
}

void nitro::RenderView::paintGL() {
    glClearColor(1,1,1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (firstPersonMode) {
        moveFirstPersonCamera();
    }
    QPainter painter(this);
    QElapsedTimer timer;
    painter.beginNativePainting();
    timer.start();
    renderer.draw();
    currentTime += timer.nsecsElapsed();
    painter.endNativePainting();
    frameCount++;

    drawHud(painter);

    painter.end();
}

void nitro::RenderView::moveFirstPersonCamera() {
    QPointF diff = QCursor::pos() - widgetMidPoint;
    diff *= mouseSensitivity;

    camYaw -= diff.x();
    camPitch -= diff.y();
    camPitch = std::clamp(camPitch, -90.0f, 90.0f);
    QCursor::setPos(widgetMidPoint);
    QMatrix4x4 rot;
    rot.rotate(QQuaternion::fromEulerAngles(camPitch, camYaw, 0));

    QVector4D movDif = QVector4D(0, 0, 0, 0);
    if (keys[Qt::Key_W]) { // Forward
        movDif.setZ(movementSpeed);
    }
    if (keys[Qt::Key_A]) { // Left
        movDif.setX(movementSpeed);
    }
    if (keys[Qt::Key_Q]) { // Down
        movDif.setY(movementSpeed);
    }
    if (keys[Qt::Key_S]) { // Backward
        movDif.setZ(movDif.z() - movementSpeed);
    }
    if (keys[Qt::Key_D]) { // Right
        movDif.setX(movDif.x() - movementSpeed);
    }
    if (keys[Qt::Key_E]) { // Up
        movDif.setY(movDif.y() - movementSpeed);
    }
    translation -= (rot * movDif).toVector3D();

    recalcCamMatrix();
}

void nitro::RenderView::drawHud(QPainter &painter) {
    QPen chPen;
    chPen.setColor({150, 150, 150});
    painter.setPen(chPen);
    painter.drawText(20, 20, QString("x: %1 y: %2 z: %3").arg(viewerPos.x()).arg(viewerPos.y()).arg(viewerPos.z()));

    if (firstPersonMode) {
        if (frameCount >= 30) {
            renderTime = currentTime / float(frameCount) / 1000.0;
            currentTime = 0;
            frameCount = 0;
        }
        float fps = 1000.0 / renderTime;
        painter.drawText(width() - 140, 20, QString("%1").arg(renderTime));
        painter.drawText(width() - 140, 50, QString("fps: %1").arg(fps));
        chPen.setColor({20, 20, 20});
        chPen.setWidth(2);
        painter.setPen(chPen);
        painter.drawPath(crossHair);
        chPen.setColor({220, 220, 220});
        chPen.setWidth(1);
        painter.setPen(chPen);
        painter.drawPath(crossHair);
    }
}

void nitro::RenderView::recalcCamMatrix() {
    settings.cameraMatrix.setToIdentity();
    settings.cameraMatrix.rotate(initialCamAngle, initialCamRotVec);
    settings.cameraMatrix.translate(initialCamPos);
    settings.cameraMatrix.translate(translation);
    settings.cameraMatrix.rotate(QQuaternion::fromEulerAngles(camPitch, camYaw, 0));

    viewerPos = {0, 0, 0, 1};
    viewerPos = settings.cameraMatrix * viewerPos;
    settings.uniformUpdateRequired = true;
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
    // TODO: better orbit controls
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
    if (firstPersonMode) {
        float speedModStrength = event->angleDelta().y() > 0 ? 1.2 : 0.8;
        movementSpeedModifier *= speedModStrength;
        if (movementSpeedModifier < 0) {
            movementSpeedModifier = 0;
        }
        movementSpeed = baseMovementSpeed * sprintModifier * movementSpeedModifier;
        return;
    }
    float zoomFactor = 15.0;
    float zoom = event->angleDelta().y() > 0 ? zoomFactor : -zoomFactor;
    scale += zoom;

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
        if (event->key() == Qt::Key_F || event->key() == Qt::Key_AsciiTilde) {
            toggleFirstPerson();
            return;
        }
        movementSpeed = baseMovementSpeed * sprintModifier * movementSpeedModifier;
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
        movementSpeed = baseMovementSpeed * movementSpeedModifier;
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
    update();

}

void nitro::RenderView::enableFirstPerson() {
    setCursor(Qt::BlankCursor);
    QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
    oldMouseCoords = toNormalizedScreenCoordinates(width() / 2.0f, height() / 2.0f);
    frameTimer = startTimer(1000 / 60);
    firstPersonMode = true;
}


void nitro::RenderView::timerEvent(QTimerEvent *event) {
    update();
}

QMenu *nitro::RenderView::createContextMenu() {
    // TODO: different class
    auto *menu = new QMenu();
    menu->addAction("Align Top", [this] {
        alignCam({0, 0, 0}, 90);
    });
    menu->addAction("Align Front", [this] {
        alignCam({1, 0, 0});
    });
    menu->addSeparator();

    menu->addAction("First Person Mode", [this] {
        toggleFirstPerson();
    }, QKeySequence(Qt::SHIFT | Qt::Key_F));

    menu->addAction("Reset", [this] {
        disableFirstPerson();
        resetOrientation();
        updateMatrices();
    }, QKeySequence(Qt::Key_R));
    // These shortcut actions don't seem to work, but they are handled elsewhere. This just ensures they are displayed
    // in the context menu
    return menu;
}

void nitro::RenderView::alignCam(QVector3D axis, float angle) {
    resetOrientation();
    // TODO: reset orthographic
    initialCamRotVec = axis;
    initialCamAngle = angle;
    updateMatrices();
}

void nitro::RenderView::contextMenuEvent(QContextMenuEvent *event) {
    if (firstPersonMode) {
        return;
    }
    QMenu *menu = createContextMenu();
    if (menu) {
        menu->exec(event->globalPos());
    }
}

void nitro::RenderView::toggleOrthographic() {
    settings.orthographic = !settings.orthographic;
    settings.uniformUpdateRequired = true;
    update();

}

void nitro::RenderView::toggleImageColors() {
    settings.imageColors = !settings.imageColors;
    settings.uniformUpdateRequired = true;
    update();
}

void nitro::RenderView::onMessageLogged(QOpenGLDebugMessage message) {
    if(message.severity() == QOpenGLDebugMessage::LowSeverity || message.severity() == QOpenGLDebugMessage::NotificationSeverity) {
        return;
    }
    qDebug() << " → Log:" << message;
}
