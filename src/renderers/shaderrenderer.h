#ifndef SHADER_RENDERER_H
#define SHADER_RENDERER_H

#include <QOpenGLShaderProgram>

#include "renderer.h"

/**
 * @brief The MeshRenderer class is responsible for rendering a mesh. Can render
 * any arbitrary mesh.
 */
class ShaderRenderer : public Renderer {
 public:
  ShaderRenderer();
  ~ShaderRenderer() override;

  void updateUniforms();
  void updateBuffers(const QImage& image);
  void draw() override;

 protected:
  void initShaders() override;
  void initBuffers() override;

 private:
  QVector<quint8> imageToBytes(const QImage& image) const;
  GLuint vao;
  GLuint coordsBO, indexBO;
  GLuint textureBO;
  int meshIBOSize;

  GLint imageUniform;
};

#endif  // SHADER_RENDERER_H
