#ifndef MESH_VIEW_H
#define MESH_VIEW_H

#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QVector3D>

#include "renderers/surfacerenderer.h"

class MeshView : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {
  Q_OBJECT

 public:
  MeshView(QWidget* parent = nullptr);
  ~MeshView() override;
  void updateMatrices();
  void updateUniforms();
  void updateProjectionMatrix();

 protected:
  void initializeGL() override;
  void resizeGL(int newWidth, int newHeight) override;
  void paintGL() override;

  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

 private:
  QOpenGLDebugLogger debugLogger;
  // for zoom
  float scale;
  // for handling rotation
  QVector3D oldRotationVec;
  QVector2D oldMouseCoords;
  QQuaternion rotationQuaternion;
  QVector3D translation;
  bool dragging;
  bool shiftPressed;

  SurfaceRenderer surfaceRenderer;
  Settings settings;
  // save this matrix here to prevent recalculating this every time the mouse is
  // pressed.
  QMatrix4x4 toWorldCoordsMatrix;

  // we make mainwindow a friend so it can access settings
  friend class MainWindow;

  void resetModelViewMatrix();
  void resetOrientation();
  QVector2D toNormalizedScreenCoordinates(float x, float y) const;

  void mouseMoveRotate(QMouseEvent* Event);
  void mouseMoveTranslate(QMouseEvent* Event);
 private slots:
  void onMessageLogged(QOpenGLDebugMessage Message);
};

#endif  // MESH_VIEW_H
