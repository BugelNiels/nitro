#include "meshview.h"

#include <QLoggingCategory>
#include <QMouseEvent>
#include <QOpenGLVersionFunctionsFactory>

#include "math.h"

MeshView::MeshView(QWidget* Parent)
    : QOpenGLWidget(Parent), shiftPressed(false) {
  resetOrientation();
}

MeshView::~MeshView() { makeCurrent(); }

void MeshView::initializeGL() {
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
  glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
  qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

  makeCurrent();

  // Enable depth buffer
  glEnable(GL_DEPTH_TEST);
  // Default is GL_LESS
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // grab the opengl context
  QOpenGLFunctions_4_1_Core* functions =
      QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_1_Core>(
          this->context());

  // initialize renderers here with the current context
  surfaceRenderer.init(functions, &settings);

  updateMatrices();
}

void MeshView::resizeGL(int newWidth, int newHeight) {
  settings.view.dispRatio = float(newWidth) / float(newHeight);

  settings.projectionMatrix.setToIdentity();
  settings.projectionMatrix.perspective(settings.view.fov,
                                        settings.view.dispRatio, 0.5f, 10.0f);
  updateMatrices();
}

void MeshView::updateProjectionMatrix() {
  settings.projectionMatrix.setToIdentity();
  settings.projectionMatrix.perspective(settings.view.fov,
                                        settings.view.dispRatio, 0.5f, 10.0f);
}

void MeshView::resetModelViewMatrix() {
  settings.modelViewMatrix.setToIdentity();
  settings.modelViewMatrix.translate(translation);
}

void MeshView::resetOrientation() {
  scale = 1.0f;
  translation = QVector3D(0.0, 0.0, -settings.view.distFromCamera);
  rotationQuaternion = QQuaternion();
}

void MeshView::updateMatrices() {
  resetModelViewMatrix();
  settings.modelViewMatrix.scale(scale);
  settings.modelViewMatrix.rotate(rotationQuaternion);

  settings.normalMatrix = settings.modelViewMatrix.normalMatrix();
  // create the matrix to map clipping space coordinates to world space
  // coordinates. Do this calculation here to prevent redoing this calculating
  // everytime the mouse is pressed. Only needs to be updated whenever the
  // projection or the modelview matrix is being updated
  bool inverted = false;
  toWorldCoordsMatrix = (settings.projectionMatrix * settings.modelViewMatrix)
                            .inverted(&inverted);

  settings.uniformUpdateRequired = true;

  update();
}

void MeshView::paintGL() {
  QVector3D bCol = settings.view.cols.background;
  glClearColor(bCol.x(), bCol.y(), bCol.z(), 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (settings.view.wireframeMode) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  surfaceRenderer.draw();
}

/**
 * @brief MainView::toNormalizedScreenCoordinates Normalizes the mouse
 * coordinates to screen coordinates.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return A vector containing the normalized x and y screen coordinates.
 */
QVector2D MeshView::toNormalizedScreenCoordinates(float x, float y) const {
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
 * @brief MeshView::mouseMoveRotate Updates the camera rotation based on mouse
 * movement.
 * @param Event Mouse event.
 */
void MeshView::mouseMoveRotate(QMouseEvent* event) {
  QVector2D sPos = toNormalizedScreenCoordinates(event->position().x(),
                                                 event->position().y());
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
    oldRotationVec = newVec;
    return;
  }

  // calculate axis and angle
  QVector3D v1 = oldRotationVec.normalized();
  QVector3D N = QVector3D::crossProduct(v1, v2).normalized();
  if (N.length() == 0.0f) {
    oldRotationVec = newVec;
    return;
  }
  float angle = 180.0f / M_PI * acos(QVector3D::dotProduct(v1, v2));
  rotationQuaternion =
      QQuaternion::fromAxisAndAngle(N, angle) * rotationQuaternion;
  updateMatrices();

  // for next iteration
  oldRotationVec = newVec;
}

/**
 * @brief MeshView::mouseMoveTranslate Updates camera translation based on mouse
 * movement.
 * @param event Mouse event.
 */
void MeshView::mouseMoveTranslate(QMouseEvent* event) {
  QVector2D sPos = toNormalizedScreenCoordinates(event->position().x(),
                                                 event->position().y());
  if (!dragging) {
    dragging = true;
    oldMouseCoords = sPos;
    return;
  }
  QVector3D translationUpdate = QVector3D(sPos.x() - oldMouseCoords.x(),
                                          sPos.y() - oldMouseCoords.y(), 0.0);
  translationUpdate *= settings.view.dragSensitivity;
  translation += translationUpdate;
  updateMatrices();
  oldMouseCoords = sPos;
}

/**
 * @brief MeshView::mouseMoveEvent Event that is called when the mouse is moved.
 * @param Event The mouse event.
 */
void MeshView::mouseMoveEvent(QMouseEvent* event) {
  if (event->buttons() == Qt::LeftButton) {
    if (shiftPressed) {
      oldRotationVec = QVector3D();
      mouseMoveTranslate(event);
    } else {
      mouseMoveRotate(event);
    }
  } else {
    dragging = false;
    oldRotationVec = QVector3D();
  }
}

/**
 * @brief MainView::mousePressEvent Event that is called when the mouse is
 * pressed.
 * @param event The mouse event.
 */
void MeshView::mousePressEvent(QMouseEvent* event) { setFocus(); }

/**
 * @brief MainView::wheelEvent Event that is called when the user scrolls.
 * @param event The mouse event.
 */
void MeshView::wheelEvent(QWheelEvent* event) {
  float phi = 1.0f + (event->angleDelta().y() / 2000.0f);
  scale = fmin(fmax(phi * scale, 0.01f), 100.0f);
  updateMatrices();
}

/**
 * @brief MainView::keyPressEvent Event that is called when a key is pressed.
 * @param event The key event.
 */
void MeshView::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case 'Z':
      settings.view.wireframeMode = !settings.view.wireframeMode;
      update();
      break;
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
void MeshView::keyReleaseEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Shift:
      shiftPressed = false;
  }
}

/**
 * @brief MainView::onMessageLogged Helper function for logging messages.
 * @param Message The message to log.
 */
void MeshView::onMessageLogged(QOpenGLDebugMessage Message) {
  qDebug() << " → Log:" << Message;
}
