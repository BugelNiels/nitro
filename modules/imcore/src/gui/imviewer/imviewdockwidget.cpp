#include <QHBoxLayout>
#include "imviewdockwidget.hpp"
#include "imviewer.hpp"
#include "gui/mainwindow.hpp"
#include "gui/zoombar.hpp"

nitro::ImViewDockWidget::ImViewDockWidget(ImageViewer *imageViewer, MainWindow *window)
        : QDockWidget(window), imageViewer_(imageViewer) {
    setWindowTitle("Image Viewer");

    QWidget *imViewTitleWrapper = initTitleBarWidget(window);


    QObject::connect(imageViewer_, &nitro::ImageViewer::mousePosUpdated, window,
                     [this](const QPoint &pos, QColor color) {
                         updateFooterLabels(pos, color);
                     });


    setTitleBarWidget(imViewTitleWrapper);

    QWidget *wrapper = new QWidget();

    auto mainLayout = new QVBoxLayout();
    mainLayout->addWidget(imageViewer_);
    imageViewer_->adjustSize();
    mainLayout->addWidget(initStatusBar());
    wrapper->setLayout(mainLayout);


    setWidget(wrapper);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setFeatures(features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));

    colLabel_->adjustSize();
    updateFooterLabels({0, 0}, {0, 0, 0});
}

QWidget *nitro::ImViewDockWidget::initTitleBarWidget(nitro::MainWindow *window) const {
    auto imViewTitleWrapper = new QWidget();
    auto imHLayout = new QHBoxLayout();

    imHLayout->addWidget(window->buildDockIcon(":/icons/image_viewer.png"));

    auto zoomBar = new ZoomBar(imageViewer_->minScaleFactor * 100.0, imageViewer_->maxScaleFactor * 100.0);
    zoomBar->setEnabled(false);
    auto sizeLabel = new QLabel("0 x 0");
    sizeLabel->setFixedWidth(100);
    auto channelsLabel = new QLabel("-");
    channelsLabel->setFixedWidth(spacing_);
    zoomBar->setMaximumWidth(200);
    auto zoomLabel = new QLabel("zoom:");
    imHLayout->addStretch();
    imHLayout->addWidget(zoomLabel);
    imHLayout->addWidget(zoomBar);
    imHLayout->addSpacing(spacing_);
    imHLayout->addWidget(new QLabel("size: "));
    imHLayout->addWidget(sizeLabel);
    imHLayout->addSpacing(spacing_);
    imHLayout->addWidget(new QLabel("channels: "));
    imHLayout->addWidget(channelsLabel);


    connect(imageViewer_, &nitro::ImageViewer::scaleChanged, window,
            [zoomBar](double scale) {
                zoomBar->setZoom(scale);
            });
    connect(imageViewer_, &nitro::ImageViewer::imageUpdated, window,
            [sizeLabel, channelsLabel](const ImageInfo &img) {
                sizeLabel->setText(QString("%1 x %2").arg(img.width).arg(img.height));
                channelsLabel->setText(QString("%1").arg(img.channels));
            });

    imViewTitleWrapper->setLayout(imHLayout);
    return imViewTitleWrapper;
}

void nitro::ImViewDockWidget::updateFooterLabels(const QPoint &pos, const QColor &color) {
    xLabel_->setText(QString::number(pos.x()));
    yLabel_->setText(QString::number(pos.y()));
    rLabel_->setText(QString::number(color.redF(), 'f', 3));
    gLabel_->setText(QString::number(color.greenF(), 'f', 3));
    bLabel_->setText(QString::number(color.blueF(), 'f', 3));
    hLabel_->setText(QString::number(color.hueF(), 'f', 3));
    sLabel_->setText(QString::number(color.saturationF(), 'f', 3));
    vLabel_->setText(QString::number(color.valueF(), 'f', 3));
    lLabel_->setText(QString::number(color.lightnessF(), 'f', 3));
    hexLabel_->setText(color.name());

    if (colLabelPixMap_.size() != colLabel_->size()) {
        colLabelPixMap_ = QPixmap(colLabel_->width(), colLabel_->height());
    }
    colLabelPixMap_.fill(color);
    colLabel_->setPixmap(colLabelPixMap_);
}

QWidget *nitro::ImViewDockWidget::initStatusBar() {

    auto *statusBar = new QWidget(this);
    auto *layout = new QHBoxLayout();

    colLabel_ = new QLabel();
    colLabel_->setFixedWidth(spacing_);
    xLabel_ = new QLabel("-");
    yLabel_ = new QLabel("-");
    rLabel_ = new QLabel("-");
    gLabel_ = new QLabel("-");
    bLabel_ = new QLabel("-");
    hLabel_ = new QLabel("-");
    sLabel_ = new QLabel("-");
    vLabel_ = new QLabel("-");
    lLabel_ = new QLabel("-");
    hexLabel_ = new QLabel("-");


    xLabel_->setFixedWidth(spacing_);
    yLabel_->setFixedWidth(spacing_);
    rLabel_->setFixedWidth(spacing_);
    gLabel_->setFixedWidth(spacing_);
    bLabel_->setFixedWidth(spacing_);
    hLabel_->setFixedWidth(spacing_);
    sLabel_->setFixedWidth(spacing_);
    vLabel_->setFixedWidth(spacing_);
    lLabel_->setFixedWidth(spacing_);
    hexLabel_->setTextInteractionFlags(Qt::TextSelectableByMouse);

    layout->addSpacing(spacing_);
    layout->addWidget(new QLabel("Color: "));
    layout->addWidget(colLabel_);
    layout->addSpacing(spacing_);


    layout->addWidget(new QLabel("X: "));
    layout->addWidget(xLabel_);
    layout->addWidget(new QLabel("Y: "));
    layout->addWidget(yLabel_);

    layout->addSpacing(spacing_);

    layout->addWidget(new QLabel("R: "));
    layout->addWidget(rLabel_);
    layout->addWidget(new QLabel("G: "));
    layout->addWidget(gLabel_);
    layout->addWidget(new QLabel("B: "));
    layout->addWidget(bLabel_);

    layout->addSpacing(spacing_);

    layout->addWidget(new QLabel("H: "));
    layout->addWidget(hLabel_);
    layout->addWidget(new QLabel("S: "));
    layout->addWidget(sLabel_);
    layout->addWidget(new QLabel("V: "));
    layout->addWidget(vLabel_);
    layout->addWidget(new QLabel("L: "));
    layout->addWidget(lLabel_);


    layout->addSpacing(spacing_);

    layout->addWidget(new QLabel("Hex: "));
    layout->addWidget(hexLabel_);

    layout->addStretch();
    statusBar->setStyleSheet("color: grey;");
    statusBar->setLayout(layout);
    return statusBar;
}

nitro::ImViewDockWidget::~ImViewDockWidget() = default;
