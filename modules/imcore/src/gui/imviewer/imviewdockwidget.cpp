#include "imviewdockwidget.hpp"
#include "imageviewer.hpp"
#include "src/gui/histogram/histogramviewer.hpp"

#include <gui/mainwindow.hpp>
#include <gui/zoombar.hpp>
#include <QTabBar>
#include <QHBoxLayout>

namespace nitro::ImCore {

const int SPACING = 10;

static QWidget *spacing() {
    auto label = new QLabel("|");
    label->setContentsMargins(SPACING, 0, SPACING, 0);
    return label;
}

ImViewDockWidget::ImViewDockWidget(ImageViewer *imageViewer, MainWindow *window)
        : QDockWidget(window), imageViewer_(imageViewer) {
    setWindowTitle("Image Viewer");

    QWidget *imViewTitleWrapper = initTitleBarWidget(window);


    setTitleBarWidget(imViewTitleWrapper);

    setWidget(imageViewer_);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setFeatures(features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
}

QWidget *ImViewDockWidget::initTitleBarWidget(MainWindow *window) const {
    auto imViewTitleWrapper = new QWidget();
    auto imHLayout = new QHBoxLayout();

    imHLayout->addWidget(window->buildDockIcon(":/icons/image_viewer.png"));

    auto zoomBar = new ZoomBar(imageViewer_->minScaleFactor * 100.0, imageViewer_->maxScaleFactor * 100.0);
    zoomBar->setEnabled(false);
    auto sizeLabel = new QLabel("0 x 0");
    sizeLabel->setFixedWidth(100);
    zoomBar->setMaximumWidth(200);
    auto zoomLabel = new QLabel("zoom:");
    imHLayout->addStretch();
    imHLayout->addWidget(zoomLabel);
    imHLayout->addWidget(zoomBar);
    imHLayout->addWidget(spacing());
    imHLayout->addWidget(new QLabel("size: "));
    imHLayout->addWidget(sizeLabel);


    connect(imageViewer_, &ImageViewer::scaleChanged, window,
            [zoomBar](double scale) {
                zoomBar->setZoom(scale);
            });
    connect(imageViewer_, &ImageViewer::imageUpdated, window,
            [sizeLabel](const cv::Mat &img) {
                sizeLabel->setText(QString("%1 x %2").arg(img.rows).arg(img.cols));
            });

    imViewTitleWrapper->setLayout(imHLayout);
    return imViewTitleWrapper;
}

ImViewDockWidget::~ImViewDockWidget() = default;

} // namespace nitro::ImCore
