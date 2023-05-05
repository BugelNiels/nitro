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

        QMap<int, bool> keys;

        // TODO: this will be passed by reference in the constructor
        Settings settings;

        bool firstPersonMode = false;

        QVector3D oldTranslation;

        // for handling rotation
        QVector3D oldRotationVec;
        QVector2D oldMouseCoords;
        bool dragging;

        ShaderRenderer renderer;

        void resetModelViewMatrix();

        void resetOrientation();

        QVector2D toNormalizedScreenCoordinates(float x, float y) const;

        void mouseMoveRotate(QMouseEvent *Event);

        void mouseMoveTranslate(QMouseEvent *Event);

        void mouseReleaseEvent(QMouseEvent *event) override;

        void toggleFirstPerson();

        void disableFirstPerson();

        void enableFirstPerson();

        void timerEvent(QTimerEvent *event) override;

        int frameTimer = -1;
        float movementSpeed = 1;
        QPoint widgetMidPoint;
        QVector3D translation;
        float camYaw = 0;
        float camPitch =  0;
    };

}  // namespace nitro
