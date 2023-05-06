#pragma once

#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QVector3D>
#include <QMenu>
#include <QPainterPath>

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

        void toggleOrthographic();

        void toggleImageColors();

    protected:
        void initializeGL() override;

        void resizeGL(int newWidth, int newHeight) override;

        void paintGL() override;

        void mouseMoveEvent(QMouseEvent *event) override;

        void mousePressEvent(QMouseEvent *event) override;

        void wheelEvent(QWheelEvent *event) override;

        void keyPressEvent(QKeyEvent *event) override;

        void keyReleaseEvent(QKeyEvent *event) override;

        void mouseReleaseEvent(QMouseEvent *event) override;

        void timerEvent(QTimerEvent *event) override;

        void contextMenuEvent(QContextMenuEvent *event) override;


    private slots:
        void onMessageLogged(QOpenGLDebugMessage Message);

    private:

        QOpenGLDebugLogger debugLogger;
        QMap<int, bool> keys;


        int frameTimer = -1;
        float baseMovementSpeed = 3;
        float movementSpeedModifier = 1;
        float sprintModifier = 2;

        float movementSpeed = 1;
        QPoint widgetMidPoint;
        QVector3D translation;
        float camYaw = 0;
        float camPitch = 0;

        float mouseSensitivity = 0.4;

        float scale = 1.0f;

        QVector3D initialCamPos;
        QVector3D initialCamRotVec;
        float initialCamAngle;

        // TODO: this will be passed by reference in the constructor
        Settings settings;

        bool firstPersonMode = false;

        QVector3D oldTranslation;

        // for handling rotation
        QVector3D oldRotationVec;
        QVector2D oldMouseCoords;
        bool dragging;

        ShaderRenderer renderer;

        void resetOrientation();

        QVector2D toNormalizedScreenCoordinates(float x, float y) const;

        void mouseMoveRotate(QMouseEvent *Event);

        void mouseMoveTranslate(QMouseEvent *Event);


        void toggleFirstPerson();

        void disableFirstPerson();

        void enableFirstPerson();

        QMenu *createContextMenu();


        void alignCam(QVector3D axis, float angle = 90);

        void recalcCamMatrix();

        qint64 currentTime;
        int frameCount = 0;
        float renderTime = 0;
        QPainterPath crossHair;
        QVector4D viewerPos;

        void drawHud(QPainter &painter);

        void moveFirstPersonCamera();
    };

}  // namespace nitro
