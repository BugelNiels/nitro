#include "shaderrenderer.hpp"

#include <QGuiApplication>
#include <QImage>
#include <QMatrix4x4>
#include <QScreen>
#include <iostream>

namespace nitro::Thesis {

// 6 for rendering a quad
ShaderRenderer::ShaderRenderer() : meshIBOSize(6), texture(nullptr) {}

ShaderRenderer::~ShaderRenderer() {
    vao->destroy();
    coordsBO->destroy();
    indexBO->destroy();
    texture->destroy();
    delete coordsBO;
    delete indexBO;
    delete shader;
}

void ShaderRenderer::initShaders() {
    shader = constructDefaultShader("raycast");
}

void ShaderRenderer::initBuffers() {
    vao = new QOpenGLVertexArrayObject();
    vao->create();
    vao->bind();

    coordsBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    coordsBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    coordsBO->create();
    coordsBO->bind();
    indexBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    indexBO->create();
    indexBO->bind();

    shader->enableAttributeArray(0);
    shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    QVector<QVector3D> quad = {{-1.0, -1.0, 0.0},
                               {1.0, -1.0, 0.0},
                               {-1.0, 1.0, 0.0},
                               {1.0, 1.0, 0.0}};
    coordsBO->allocate(quad.data(), sizeof(QVector3D) * quad.size());

    QVector<unsigned int> meshIndices = {0, 1, 2, 1, 3, 2};
    indexBO->allocate(meshIndices.data(), sizeof(int) * meshIndices.size());
}

void ShaderRenderer::updateBuffers(const QImage &image) {
    m_imWidth = image.width();
    m_imHeight = image.height();

    texture = std::make_unique<QOpenGLTexture>(image.mirrored());
    texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    texture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    texture->bind();

    shader->bind();
    shader->setUniformValue("image", 0);
    shader->setUniformValue("imWidth", m_imWidth);
    shader->setUniformValue("imHeight", m_imHeight);
    settings->uniformUpdateRequired = true;
}

void ShaderRenderer::updateUniforms() {
    shader->bind();
    if (texture) {
        texture->bind();
    }
    shader->setUniformValue("projectionMatrix", settings->projectionMatrix);
    shader->setUniformValue("toWorldMatrix", settings->cameraMatrix);
    shader->setUniformValue("enableImageColors", settings->imageColors);
    shader->setUniformValue("enableOrthographic", settings->orthographic);
    shader->setUniformValue("minecraft", settings->minecraft);
}

void ShaderRenderer::draw() {
    vao->bind();
    if (settings->uniformUpdateRequired) {
        updateUniforms();
    }
    gl->glDrawElements(GL_TRIANGLES, meshIBOSize, GL_UNSIGNED_INT, nullptr);
}

} // namespace nitro::Thesis
