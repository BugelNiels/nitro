#pragma once

#include "gui/im3dviewer/settings.hpp"
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>

namespace nitro::Thesis {
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

    [[nodiscard]] QOpenGLShaderProgram *constructDefaultShader(const QString &name) const;

    void init(QOpenGLFunctions_4_1_Core *f, Settings *s);

    virtual void draw() = 0;

protected:
    QOpenGLFunctions_4_1_Core *gl;
    Settings *settings;
};

} // namespace nitro::Thesis
