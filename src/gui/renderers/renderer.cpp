#include "renderer.hpp"

/**
 * @brief Renderer::Renderer Creates a new renderer.
 */
Renderer::Renderer() : gl(nullptr) {}

/**
 * @brief Renderer::~Renderer Deconstructs the renderer by deleting all shaders.
 */
Renderer::~Renderer() { qDeleteAll(shaders); }

/**
 * @brief Renderer::init Initialises the renderer with an OpenGL context and
 * settings. Also initialises the shaders and buffers.
 * @param f OpenGL functions pointer.
 * @param s Settings.
 */
void Renderer::init(QOpenGLFunctions_4_1_Core* f, Settings* s) {
  gl = f;
  settings = s;

  initShaders();
  initBuffers();
}

/**
 * @brief Renderer::constructDefaultShader Constructs a shader consisting of a
 * vertex shader and a fragment shader. The shaders are assumed to follow the
 * naming convention: <name>.vert and <name>.frag. Both of these files have to
 * exist for this function to work successfully.
 * @param name Name of the shader.
 * @return The constructed shader.
 */
QOpenGLShaderProgram* Renderer::constructDefaultShader(
    const QString& name) const {
  QString pathVert = ":/shaders/" + name + ".vert";
  QString pathFrag = ":/shaders/" + name + ".frag";
  QString pathShading = ":/shaders/shading.glsl";

  // we use the qt wrapper functions for shader objects
  QOpenGLShaderProgram* shader = new QOpenGLShaderProgram();
  shader->addShaderFromSourceFile(QOpenGLShader::Vertex, pathVert);
  shader->addShaderFromSourceFile(QOpenGLShader::Fragment, pathFrag);
  shader->addShaderFromSourceFile(QOpenGLShader::Fragment, pathShading);
  shader->link();
  return shader;
}

/**
 * @brief Renderer::intUniform Updates an integer uniform location.
 * @param shader Shader.
 * @param shaderUniName Uniform name in the shader.
 * @param val Value to update the uniform with.
 */
void Renderer::intUniform(QOpenGLShaderProgram* shader,
                          const char* shaderUniName, int val) {
  GLint uni = shader->uniformLocation(shaderUniName);
  gl->glUniform1i(uni, val);
}

/**
 * @brief Renderer::floatUniform Updates a float uniform location.
 * @param shader Shader
 * @param shaderUniName Uniform name in the shader.
 * @param val Value to update the uniform with.
 */
void Renderer::floatUniform(QOpenGLShaderProgram* shader,
                            const char* shaderUniName, float val) {
  GLint uni = shader->uniformLocation(shaderUniName);
  gl->glUniform1f(uni, val);
}

/**
 * @brief Renderer::vec3Uniform Updates a vec3 uniform location.
 * @param shader Shader
 * @param shaderUniName Uniform name in the shader.
 * @param vec Vector to update the uniform with.
 */
void Renderer::vec3Uniform(QOpenGLShaderProgram* shader,
                           const char* shaderUniName, const QVector3D& vec) {
  GLint uni = shader->uniformLocation(shaderUniName);
  gl->glUniform3f(uni, vec.x(), vec.y(), vec.z());
}

/**
 * @brief Renderer::mat3Uniform Updates a mat3 uniform location.
 * @param shader Shader
 * @param shaderUniName Uniform name in the shader.
 * @param vec Matrix to update the uniform with.
 */
void Renderer::mat3Uniform(QOpenGLShaderProgram* shader,
                           const char* shaderUniName, const QMatrix3x3& mat) {
  GLint uni = shader->uniformLocation(shaderUniName);
  gl->glUniformMatrix3fv(uni, 1, false, mat.data());
}

/**
 * @brief Renderer::mat4Uniform Updates a mat4 uniform location.
 * @param shader Shader
 * @param shaderUniName Uniform name in the shader.
 * @param vec Matrix to update the uniform with.
 */
void Renderer::mat4Uniform(QOpenGLShaderProgram* shader,
                           const char* shaderUniName, const QMatrix4x4& mat) {
  GLint uni = shader->uniformLocation(shaderUniName);
  gl->glUniformMatrix4fv(uni, 1, false, mat.data());
}

/**
 * @brief Renderer::floatUniform Updates a vec3 uniform location.
 * @param shader Shader
 * @param shaderUniName Uniform name in the shader.
 * @param vec Vector to update the uniform with.
 */
void Renderer::textureUniform(QOpenGLShaderProgram* shader,
                              const char* shaderUniName, int index) {
  GLint uni = shader->uniformLocation(shaderUniName);
  gl->glUniform1i(uni, index);
}
