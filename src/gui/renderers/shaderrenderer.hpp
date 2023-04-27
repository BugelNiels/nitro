#pragma once

#include <QOpenGLShaderProgram>

#include "renderer.hpp"

namespace nitro {

/**
 * @brief The MeshRenderer class is responsible for rendering a mesh. Can render
 * any arbitrary mesh.
 */
class ShaderRenderer : public Renderer {
 public:
  ShaderRenderer();
  ~ShaderRenderer() override;

  void updateUniforms();
  void updateBuffers(const QImage &image);
  void draw() override;

 protected:
  void initShaders() override;
  void initBuffers() override;

 private:
  QVector<quint8> imageToBytes(const QImage &image) const;
  GLuint vao;
  GLuint coordsBO, indexBO;
  GLuint textureBO;
  int meshIBOSize;

  GLint imageUniform;
};
}  // namespace nitro
