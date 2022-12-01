#ifndef MESH_VIEW_H
#define MESH_VIEW_H

#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QVector3D>

/**
 * @brief The MainView class represents the main view of the UI.
 */
class MeshView : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {
  Q_OBJECT

 public:
  MeshView(QWidget* parent = nullptr);
  ~MeshView() override;

  QVector3D convertHSLtoRGB(float H, float S, float L);
  void updateBuffers(unsigned short n);
  void clearArrays();

 protected:
  void initializeGL() override;
  void resizeGL(int newWidth, int newHeight) override;
  void paintGL() override;

 private:
  QOpenGLDebugLogger* debugLogger;

  QOpenGLShaderProgram* mainShaderProg;
  GLuint fanVAO, fanCoordsBO, fanColourBO, fanIndexBO;

  void createShaderPrograms();
  void initBuffers();
  void updateMatrices();
  void updateUniforms();

  bool updateUniformsRequired;

  QVector<QVector2D> triaCoords;
  QVector<QVector3D> triaColours;
  QVector<unsigned short> triaIndices;

  GLint uniModelViewMatrix, uniProjectionMatrix;
  QMatrix4x4 modelViewMatrix, projectionMatrix;

 private slots:
  void onMessageLogged(QOpenGLDebugMessage Message);
};

#endif  // MESH_VIEW_H
