#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <QOpenGLShaderProgram>

#include "renderer.h"

/**
 * @brief The MeshRenderer class is responsible for rendering a mesh. Can render
 * any arbitrary mesh.
 */
class SurfaceRenderer : public Renderer {
 public:
  SurfaceRenderer();
  ~SurfaceRenderer() override;

  void updateUniforms();

  void initShaders() override;
  void initBuffers() override;
  void updateBuffers();
  void draw();

 private:
  GLuint vao;
  GLuint coordsBO, indexBO;
  GLuint* textureBOs;
  QOpenGLShaderProgram* raycastShader;
  int meshIBOSize;
};

#endif  // MESHRENDERER_H
