#pragma once

#include <QLabel>
#include <QOpenGLDebugLogger>
#include <QOpenGLWidget>
#include <QScrollArea>
#include <QVector3D>
#include "src/components/resamplers/resampler.hpp"
#include <QMenu>
#include <QGraphicsView>

#include "cbdimage.hpp"
#include "util/matrix.hpp"
#include "util/progressupdater.hpp"

namespace nitro {

    class ImageViewer : public QGraphicsView, public nitro::ProgressUpdater {
    Q_OBJECT


    public:
        struct ScaleRange {
            double minimum = 0;
            double maximum = 0;
        };
    public:
        explicit ImageViewer(QGraphicsScene *imScene, QWidget *parent = nullptr);

        ~ImageViewer() override;

        void wheelEvent(QWheelEvent *event) override;

        void keyPressEvent(QKeyEvent *event) override;

        void resetImScale();

        void setImage(const QImage &newImage);

        void drawBackground(QPainter *painter, const QRectF &r) override;

        void contextMenuEvent(QContextMenuEvent *event) override;

        void resizeEvent(QResizeEvent *event) override;

        void removeImage();

        void awaitReplacement();


        const double minScaleFactor = 0.2;
        const double maxScaleFactor = 20;
    protected:

        void saveImage();

    public Q_SLOTS:

        void scaleUp();

        void scaleDown();

    Q_SIGNALS:

        void scaleChanged(double scale);

    private:

        const int dotSize = 3;
        const int gridStep = 64;

        const QColor dotColor = QColor(80, 80, 80);
        const QColor gridBackgroundColor = QColor(57, 57, 57);
        const QColor bGroundCol = QColor(55, 55, 55);
        const QColor imgOutlineCol = QColor(128, 128, 128);
        const QColor imgGridCol = QColor(78, 78, 78);
        const int gridStepSize = 32;
        const int emptySize = 128;

        bool _replacementDue = false;
        ScaleRange _scaleRange;
        QImage *displayImg = nullptr;
        QGraphicsPixmapItem *_imgDisplayItem = nullptr;

        void setScaleRange(double minimum = 0, double maximum = 0);

        void setupScale(double scale);

        QMenu *createContextMenu();

        void centerScene();

        QAction *saveAction;
        QAction *resetAction;

        void initActions();

    };

} // nitro