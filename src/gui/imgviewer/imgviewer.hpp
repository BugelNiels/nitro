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
#include "settings/imviewsettings.hpp"
#include "settings/samplesettings.hpp"
#include "util/matrix.hpp"
#include "util/progressupdater.hpp"
#include "imgviewerscene.hpp"

namespace nitro {

    class ImageViewer : public QGraphicsView, public nitro::ProgressUpdater {
    Q_OBJECT


    public:
        struct ScaleRange {
            double minimum = 0;
            double maximum = 0;
        };
    public:
        explicit ImageViewer(ImageViewerScene *imScene, QWidget *parent = nullptr);

        ~ImageViewer() override;

        void wheelEvent(QWheelEvent *event) override;

        void resetImScale();

        void setImage(const QImage &newImage);

        void drawBackground(QPainter *painter, const QRectF &r) override;

        void contextMenuEvent(QContextMenuEvent *event) override;

        void resizeEvent(QResizeEvent *event) override;

        void removeImage();

    public Q_SLOTS:

        void scaleUp();

        void scaleDown();

    Q_SIGNALS:

        void scaleChanged(double scale);

    private:
        const int emptySize = 128;
        ScaleRange _scaleRange;
        QImage *displayImg = nullptr;
        QGraphicsPixmapItem *_oldImg = nullptr;

        void setScaleRange(double minimum = 0, double maximum = 0);

        void setupScale(double scale);

        QPointF _clickPos;

        QMenu *createContextMenu();

        void centerScene();

    };

} // nitro