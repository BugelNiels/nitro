#include "shaderrenderer.hpp"

#include <QGuiApplication>
#include <QImage>
#include <QMatrix4x4>
#include <QScreen>
#include <iostream>

nitro::ShaderRenderer::ShaderRenderer() : meshIBOSize(0) {}

nitro::ShaderRenderer::~ShaderRenderer() {
  gl->glDeleteVertexArrays(1, &vao);
  gl->glDeleteBuffers(1, &coordsBO);
  gl->glDeleteBuffers(1, &indexBO);
  gl->glDeleteTextures(1, &textureBO);
}

void nitro::ShaderRenderer::initShaders() {
  shaders.insert(ShaderType::SDF, constructDefaultShader("sdf"));
  shaders.insert(ShaderType::SURFACE, constructDefaultShader("raycast"));
  shaders.insert(ShaderType::LEVEL, constructDefaultShader("level"));
}

void nitro::ShaderRenderer::initBuffers() {
  gl->glGenVertexArrays(1, &vao);
  gl->glBindVertexArray(vao);

  gl->glGenBuffers(1, &coordsBO);
  gl->glBindBuffer(GL_ARRAY_BUFFER, coordsBO);
  gl->glEnableVertexAttribArray(0);
  gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  gl->glGenBuffers(1, &indexBO);
  gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO);

  gl->glGenTextures(1, &textureBO);
  gl->glBindTexture(GL_TEXTURE_2D, textureBO);
  gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  gl->glBindVertexArray(0);

  QVector<QVector3D> quad = {
      {-1.0, -1.0, 0.0}, {1.0, -1.0, 0.0}, {-1.0, 1.0, 0.0}, {1.0, 1.0, 0.0}};

  gl->glBindBuffer(GL_ARRAY_BUFFER, coordsBO);
  gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D) * quad.size(),
                   quad.data(), GL_STATIC_DRAW);

  QVector<unsigned int> meshIndices = {0, 1, 2, 1, 3, 2};

  gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO);
  gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * meshIndices.size(),
                   meshIndices.data(), GL_STATIC_DRAW);
  meshIBOSize = meshIndices.size();
}

QVector<quint8> nitro::ShaderRenderer::imageToBytes(const QImage& image) const {
  // needed since (0,0) is bottom left in OpenGL
  QImage im = image.mirrored();
  QVector<quint8> pixelData;
  pixelData.reserve(im.width() * im.height() * 4);

  for (int i = 0; i != im.height(); ++i) {
    for (int j = 0; j != im.width(); ++j) {
      QRgb pixel = im.pixel(j, i);

      // pixel is of format #AARRGGBB (in hexadecimal notation)
      // so with bitshifting and binary AND you can get
      // the values of the different components
      quint8 r = (quint8)((pixel >> 16) & 0xFF);  // Red component
      quint8 g = (quint8)((pixel >> 8) & 0xFF);   // Green component
      quint8 b = (quint8)(pixel & 0xFF);          // Blue component
      quint8 a = (quint8)((pixel >> 24) & 0xFF);  // Alpha component

      // Add them to the Vector
      pixelData.append(r);
      pixelData.append(g);
      pixelData.append(b);
      pixelData.append(a);
    }
  }
  return pixelData;
}

void nitro::ShaderRenderer::updateBuffers(const QImage& image) {
  QVector<quint8> bytes = imageToBytes(image);
  gl->glBindTexture(GL_TEXTURE_2D, textureBO);
  gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width(), image.height(), 0,
                   GL_RGBA, GL_UNSIGNED_BYTE, bytes.data());
  settings->uniformUpdateRequired = true;
}

void nitro::ShaderRenderer::updateUniforms() {
  const QVector3D& dims = settings->view.boundingBoxDims;
  QVector3D p1 = QVector3D(settings->view.modelViewMatrix *
                           QVector4D(-dims.x(), -dims.y(), -dims.z(), 1));
  QVector3D p2 = QVector3D(settings->view.modelViewMatrix *
                           QVector4D(-dims.x(), -dims.y(), dims.z(), 1));
  QVector3D p3 = QVector3D(settings->view.modelViewMatrix *
                           QVector4D(-dims.x(), dims.y(), -dims.z(), 1));
  QVector3D p4 = QVector3D(settings->view.modelViewMatrix *
                           QVector4D(dims.x(), -dims.y(), -dims.z(), 1));

  QOpenGLShaderProgram* shader = shaders[settings->view.activeShader];

  mat4Uniform(shader, "modelviewmatrix", settings->view.modelViewMatrix);
  mat4Uniform(shader, "projectionmatrix", settings->view.projectionMatrix);

  vec3Uniform(shader, "boundp1", p1);
  vec3Uniform(shader, "boundp2", p2);
  vec3Uniform(shader, "boundp3", p3);
  vec3Uniform(shader, "boundp4", p4);

  floatUniform(shader, "boxheight", dims.y() * 2);

  imageUniform = shaders[settings->view.activeShader]->uniformLocation("image");
  gl->glActiveTexture(GL_TEXTURE0);
  gl->glBindTexture(GL_TEXTURE_2D, textureBO);
  gl->glUniform1i(imageUniform, 0);

  intUniform(shader, "level", 0);
  intUniform(shader, "depth", 0);  // TODO
}

void nitro::ShaderRenderer::draw() {
  QOpenGLShaderProgram* shader = shaders[settings->view.activeShader];
  gl->glBindVertexArray(vao);
  shader->bind();
  if (settings->uniformUpdateRequired) {
    updateUniforms();
  }

  gl->glDrawElements(GL_TRIANGLES, meshIBOSize, GL_UNSIGNED_INT, nullptr);

  shader->release();
  gl->glBindVertexArray(0);
}
