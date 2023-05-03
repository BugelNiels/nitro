#pragma once

#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QVector3D>

#include "src/gui/surfacevis/renderers/renderer.hpp"
#include "src/gui/surfacevis/renderers/shaderrenderer.hpp"

namespace nitro {

    class RenderView : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {
    Q_OBJECT

    public:
        explicit RenderView(QWidget *parent = nullptr);

        ~RenderView() override;

        void updateMatrices();

        void updateUniforms();

        void updateProjectionMatrix();

        void updateBuffers(const QImage &image);

    protected:
        void initializeGL() override;

        void resizeGL(int newWidth, int newHeight) override;

        void paintGL() override;

        void mouseMoveEvent(QMouseEvent *event) override;

        void mousePressEvent(QMouseEvent *event) override;

        void wheelEvent(QWheelEvent *event) override;

        void keyPressEvent(QKeyEvent *event) override;

        void keyReleaseEvent(QKeyEvent *event) override;

    private:

        // TODO: this will be passed by reference in the constructor
        Settings settings;

        float rotAngle = 0.0f;

        // for zoom
        float scale;
        // for handling rotation
        QVector3D oldRotationVec;
        QVector2D oldMouseCoords;
        QQuaternion rotationQuaternion;
        QVector3D translation;
        bool dragging;
        bool shiftPressed;

        ShaderRenderer renderer;

        void resetModelViewMatrix();

        void resetOrientation();

        QVector2D toNormalizedScreenCoordinates(float x, float y) const;

        void mouseMoveRotate(QMouseEvent *Event);

        void mouseMoveTranslate(QMouseEvent *Event);

        void mouseReleaseEvent(QMouseEvent *event) override;
    };

}  // namespace nitro
