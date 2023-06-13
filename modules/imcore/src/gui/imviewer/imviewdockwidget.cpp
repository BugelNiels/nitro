#include <QHBoxLayout>
#include "imviewdockwidget.hpp"
#include "imviewer.hpp"
#include "gui/mainwindow.hpp"
#include "gui/zoombar.hpp"

nitro::ImViewDockWidget::ImViewDockWidget(ImageViewer *imageViewer, MainWindow *window)
        : QDockWidget(window), imageViewer_(imageViewer) {
    setWindowTitle("Image Viewer");

    auto imViewTitleWrapper = new QWidget();
    auto imHLayout = new QHBoxLayout();

    imHLayout->addWidget(window->buildDockIcon(":/icons/image_viewer.png"));

    auto zoomBar = new ZoomBar(imageViewer_->minScaleFactor * 100.0, imageViewer_->maxScaleFactor * 100.0);
    auto sizeLabel = new QLabel("0 x 0");
    sizeLabel->setFixedWidth(100);
    auto channelsLabel = new QLabel("-");
    channelsLabel->setFixedWidth(20);
    QPalette palette = zoomBar->palette();
    palette.setColor(QPalette::Highlight, QColor(60, 60, 60)); // set the color to red
    zoomBar->setPalette(palette);
    zoomBar->setMaximumWidth(200);
    auto zoomLabel = new QLabel("zoom:");
    imHLayout->addStretch();
    imHLayout->addWidget(zoomLabel);
    imHLayout->addWidget(zoomBar);
    imHLayout->addStretch();
    imHLayout->addWidget(new QLabel("size: "));
    imHLayout->addWidget(sizeLabel);
    imHLayout->addSpacing(20);
    imHLayout->addWidget(new QLabel("channels: "));
    imHLayout->addWidget(channelsLabel);
    QObject::connect(imageViewer_, &nitro::ImageViewer::scaleChanged, window,
                     [zoomBar](double scale) {
                         zoomBar->setZoom(scale);
                     });
    QObject::connect(imageViewer_, &nitro::ImageViewer::imageUpdated, window,
                     [sizeLabel, channelsLabel](const cv::Mat &img) {
                         sizeLabel->setText(QString("%1 x %2").arg(img.cols).arg(img.rows));
                         channelsLabel->setText(QString("%1").arg(img.channels()));
                     });


    imViewTitleWrapper->setLayout(imHLayout);
    setTitleBarWidget(imViewTitleWrapper);
    setWidget(imageViewer_);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setFeatures(features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));
}


nitro::ImViewDockWidget::~ImViewDockWidget() = default;
