#pragma once

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "renderer.hpp"

namespace nitro::Thesis {

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
    QOpenGLShaderProgram *shader;
    QOpenGLVertexArrayObject *vao;
    QOpenGLBuffer *coordsBO;
    QOpenGLBuffer *indexBO;
    std::unique_ptr<QOpenGLTexture> texture;
    const int meshIBOSize;
    int m_imWidth;
    int m_imHeight;
};
}  // namespace nitro::Thesis
