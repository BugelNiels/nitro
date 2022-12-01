#include "surfacerenderer.h"

#include <QGuiApplication>
#include <QMatrix4x4>
#include <QScreen>
#include <iostream>

SurfaceRenderer::SurfaceRenderer() : meshIBOSize(0) {}

SurfaceRenderer::~SurfaceRenderer() {
  gl->glDeleteVertexArrays(1, &vao);
  gl->glDeleteBuffers(1, &coordsBO);
  gl->glDeleteBuffers(1, &indexBO);
}

void SurfaceRenderer::initShaders() {
  raycastShader = constructDefaultShader("raycast");
}

void SurfaceRenderer::initBuffers() {
  gl->glGenVertexArrays(1, &vao);
  gl->glBindVertexArray(vao);

  gl->glGenBuffers(1, &coordsBO);
  gl->glBindBuffer(GL_ARRAY_BUFFER, coordsBO);
  gl->glEnableVertexAttribArray(0);
  gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  gl->glGenBuffers(1, &indexBO);
  gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO);

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

void SurfaceRenderer::updateBuffers() {}

void SurfaceRenderer::updateUniforms() {
  const QVector3D& dims = settings->view.boundingBoxDims;
  QVector3D p1 = QVector3D(settings->modelViewMatrix *
                           QVector4D(-dims.x(), -dims.y(), -dims.z(), 1));
  QVector3D p2 = QVector3D(settings->modelViewMatrix *
                           QVector4D(-dims.x(), -dims.y(), dims.z(), 1));
  QVector3D p3 = QVector3D(settings->modelViewMatrix *
                           QVector4D(-dims.x(), dims.y(), -dims.z(), 1));
  QVector3D p4 = QVector3D(settings->modelViewMatrix *
                           QVector4D(dims.x(), -dims.y(), -dims.z(), 1));

  mat4Uniform(raycastShader, "modelviewmatrix", settings->modelViewMatrix);
  mat4Uniform(raycastShader, "projectionmatrix", settings->projectionMatrix);

  vec3Uniform(raycastShader, "boundp1", p1);
  vec3Uniform(raycastShader, "boundp2", p2);
  vec3Uniform(raycastShader, "boundp3", p3);
  vec3Uniform(raycastShader, "boundp4", p4);

  floatUniform(raycastShader, "boxheight", dims.y() * 2);
}

void SurfaceRenderer::draw() {
  gl->glBindVertexArray(vao);
  raycastShader->bind();
  if (settings->uniformUpdateRequired) {
    updateUniforms();
  }

  gl->glDrawElements(GL_TRIANGLES, meshIBOSize, GL_UNSIGNED_INT, nullptr);

  raycastShader->release();
  gl->glBindVertexArray(0);
}
