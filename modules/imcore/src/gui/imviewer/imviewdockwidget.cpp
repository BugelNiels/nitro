#include "imviewdockwidget.hpp"
#include "imageviewer.hpp"
#include "src/gui/histogram/histogramviewer.hpp"

#include <QHBoxLayout>
#include <QTabBar>
#include <gui/mainwindow.hpp>
#include <gui/zoombar.hpp>

namespace nitro::ImCore {

ImViewDockWidget::ImViewDockWidget(ImageViewer *imageViewer, MainWindow *window)
    : NitroDockWidget("Image Viewer",window),
      imageViewer_(imageViewer) {

    setWidget(imageViewer_);

    initTitleBar(window);
}
void ImViewDockWidget::initTitleBar(const MainWindow *window) {
    setIcon(":/icons/image_viewer.png");

    auto zoomBar = new ZoomBar(imageViewer_->minScaleFactor * 100.0,
                               imageViewer_->maxScaleFactor * 100.0);
    zoomBar->setEnabled(false);
    auto sizeLabel = new QLabel("0 x 0");
    sizeLabel->setFixedWidth(100);
    zoomBar->setMaximumWidth(200);
    auto zoomLabel = new QLabel("zoom:");


    addTitleBarWidget(zoomLabel);
    addTitleBarWidget(zoomBar);
    addTitleBarWidget(spacing());
    addTitleBarWidget(new QLabel("size: "));
    addTitleBarWidget(sizeLabel);

    connect(imageViewer_, &ImageViewer::scaleChanged, window, [zoomBar](double scale) {
        zoomBar->setZoom(scale);
    });
    connect(imageViewer_, &ImageViewer::imageUpdated, window, [sizeLabel](const cv::Mat &img) {
        sizeLabel->setText(QString("%1 x %2").arg(img.rows).arg(img.cols));
    });
}

ImViewDockWidget::~ImViewDockWidget() = default;

} // namespace nitro::ImCore
