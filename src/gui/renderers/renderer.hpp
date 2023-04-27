#pragma once

#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>

#include "../settings/settings.hpp"
#include "../settings/shadertypes.hpp"

namespace nitro {
/**
 * @brief The Renderer class represents a generic renderer class. The class is
 * essentially abstract and should only contain functionality that is applicable
 * to every renderer.
 */
class Renderer {
 public:
  Renderer();
  Renderer(QOpenGLFunctions_4_1_Core *functions, Settings *settings);
  virtual ~Renderer();

  virtual void initShaders() = 0;
  virtual void initBuffers() = 0;

  QOpenGLShaderProgram *constructDefaultShader(const QString &name) const;
  void init(QOpenGLFunctions_4_1_Core *f, Settings *s);
  virtual void draw() = 0;

 protected:
  QMap<ShaderType, QOpenGLShaderProgram *> shaders;
  QOpenGLFunctions_4_1_Core *gl;
  Settings *settings;

  void intUniform(QOpenGLShaderProgram *shader, const char *shaderUniName, int val);
  void floatUniform(QOpenGLShaderProgram *shader, const char *shaderUniName, float val);
  void vec3Uniform(QOpenGLShaderProgram *shader, const char *shaderUniName, const QVector3D &vec);
  void mat3Uniform(QOpenGLShaderProgram *shader, const char *shaderUniName, const QMatrix3x3 &mat);
  void mat4Uniform(QOpenGLShaderProgram *shader, const char *shaderUniName, const QMatrix4x4 &mat);
  void textureUniform(QOpenGLShaderProgram *shader, const char *shaderUniName, int index);
};
}  // namespace nitro
