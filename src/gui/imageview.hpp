#pragma once

#include <QLabel>
#include <QOpenGLDebugLogger>
#include <QOpenGLWidget>
#include <QScrollArea>
#include <QVector3D>
#include <resamplers/resampler.hpp>
#include <QMenu>

#include "cbdimage.hpp"
#include "settings/imviewsettings.hpp"
#include "settings/samplesettings.hpp"
#include "util/matrix.hpp"
#include "util/progressupdater.hpp"

namespace nitro {

    class ImageView : public QScrollArea, public nitro::ProgressUpdater {
    Q_OBJECT

    public:
        explicit ImageView(QWidget *parent = nullptr);

        ~ImageView() override;

//        bool loadFile(const QString &fileName);
//
//        void quantisize();
//
//        void calcDistanceField();
//
//        void resample();
        //  CbdImage compress();
        //  void decompress(CbdImage& compressImg);

//        const QImage &getActiveDisplayImage();
//
//        const QImage &getImageByIndex(int index);

        void mouseMoveEvent(QMouseEvent *event) override;

        void mousePressEvent(QMouseEvent *event) override;

        void mouseReleaseEvent(QMouseEvent *event) override;

        void wheelEvent(QWheelEvent *event) override;

        void keyPressEvent(QKeyEvent *event) override;

        void scaleImToFit();

        void resetImScale();

//        void updateImage();
        void setImage(const QImage &newImage);

    private:

//        nitro::SampleSettings sampleSettings;
        nitro::ImViewSettings viewSettings;

        QImage* curImg;

        // vector allows for rgb
//        QVector<nitro::CbdImage> originalImg;
//        QVector<nitro::CbdImage> quantisizedImg;
        QVector<QImage> savedImages;

        QVector2D oldMouseCoords;
        QVector2D translation;
        QVector2D oldScrollValue;

        QLabel *imageLabel;
        float scale = 1.0;
        bool dragging;
//        bool quantisized;

        // we make mainwindow a friend so it can access settings
        friend class MainWindow;

        QMenu *initMenu();
    };

} // nitro