#include "renderer.hpp"
#include <QOpenGLDebugLogger>

/**
 * @brief Renderer::Renderer Creates a new renderer.
 */
nitro::Renderer::Renderer() : gl(nullptr) {}


nitro::Renderer::Renderer(QOpenGLFunctions_4_1_Core *functions, nitro::Settings *settings) {
    init(functions, settings);
}


/**
 * @brief Renderer::~Renderer
 * */
nitro::Renderer::~Renderer() { }

/**
 * @brief Renderer::updatebounds Initialises the renderer with an OpenGL context and
 * settings. Also initialises the shaders and buffers.
 * @param f OpenGL functions pointer.
 * @param s Settings.
 */
void nitro::Renderer::init(QOpenGLFunctions_4_1_Core *f, Settings *s) {
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
QOpenGLShaderProgram *nitro::Renderer::constructDefaultShader(
        const QString &name) const {
    QString pathVert = ":/shaders/" + name + ".vert";
    QString pathFrag = ":/shaders/" + name + ".frag";
    QString pathShading = ":/shaders/shading.glsl";

    // we use the qt wrapper functions for shader objects
    auto *shader = new QOpenGLShaderProgram();
    shader->create();
    shader->addShaderFromSourceFile(QOpenGLShader::Vertex, pathVert);
    shader->addShaderFromSourceFile(QOpenGLShader::Fragment, pathFrag);
    shader->addShaderFromSourceFile(QOpenGLShader::Fragment, pathShading);
    shader->link();
    return shader;
}
