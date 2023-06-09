#include <QHBoxLayout>
#include "imviewdockwidget.hpp"
#include "imgviewer.hpp"
#include "mainwindow.hpp"
#include "components/zoombar.hpp"

nitro::ImViewDockWidget::ImViewDockWidget(ImageViewer *imageViewer, MainWindow *window)
        : QDockWidget(window), imageViewer_(imageViewer) {
    setWindowTitle("Image Viewer");

    auto *imViewTitleWrapper = new QWidget();
    auto *imHLayout = new QHBoxLayout();

    imHLayout->addWidget(window->buildDockIcon(":/icons/image_viewer.png"));

    imHLayout->addStretch();

    auto *zoomLabel = new QLabel("zoom:");
    imHLayout->addWidget(zoomLabel);

    auto *zoomBar = new ZoomBar(imageViewer_->minScaleFactor * 100.0, imageViewer_->maxScaleFactor * 100.0);
    QPalette palette = zoomBar->palette();
    palette.setColor(QPalette::Highlight, QColor(60, 60, 60)); // set the color to red
    zoomBar->setPalette(palette);
    zoomBar->setMaximumWidth(200);
    imHLayout->addWidget(zoomBar);
    QObject::connect(imageViewer_, &nitro::ImageViewer::scaleChanged, window, [=](double scale) {
        zoomBar->setZoom(scale);
    });


    imViewTitleWrapper->setLayout(imHLayout);
    setTitleBarWidget(imViewTitleWrapper);
    setWidget(imageViewer_);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setFeatures(features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
}


nitro::ImViewDockWidget::~ImViewDockWidget() = default;