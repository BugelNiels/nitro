#include "imviewdockwidget.hpp"
#include "imviewer.hpp"
#include "gui/mainwindow.hpp"
#include "../../../../../src/gui/components/zoombar.hpp"

#include <QHBoxLayout>

namespace nitro::ImCore {

const int SPACING = 20;

static QWidget *spacing() {
    auto label = new QLabel("|");
    label->setContentsMargins(SPACING, 0, SPACING, 0);
    return label;
}

ImViewDockWidget::ImViewDockWidget(ImageViewer *imageViewer, MainWindow *window)
        : QDockWidget(window), imageViewer_(imageViewer) {
    setWindowTitle("Image Viewer");

    QWidget *imViewTitleWrapper = initTitleBarWidget(window);


    QObject::connect(imageViewer_, &ImageViewer::mousePosUpdated, window,
                     [this](const QPoint &pos, QColor color) {
                         updateFooterLabels(pos, color);
                     });


    setTitleBarWidget(imViewTitleWrapper);

    auto *wrapper = new QWidget();
    wrapper->setContentsMargins(0, 0, 0, 0);

    auto mainLayout = new QVBoxLayout();
    mainLayout->addWidget(imageViewer_);
    imageViewer_->adjustSize();
    mainLayout->addWidget(initStatusBar());
    mainLayout->setContentsMargins(0, 0, 0, 0);
    wrapper->setLayout(mainLayout);


    setWidget(wrapper);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setFeatures(features() & ~(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable));

    colLabel_->adjustSize();
    updateFooterLabels({0, 0}, {0, 0, 0});
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
    imHLayout->addWidget(spacing());


    connect(imageViewer_, &ImageViewer::scaleChanged, window,
            [zoomBar](double scale) {
                zoomBar->setZoom(scale);
            });
    connect(imageViewer_, &ImageViewer::imageUpdated, window,
            [sizeLabel](const ImageInfo &img) {
                sizeLabel->setText(QString("%1 x %2").arg(img.width).arg(img.height));
            });

    imViewTitleWrapper->setLayout(imHLayout);
    return imViewTitleWrapper;
}

void ImViewDockWidget::updateFooterLabels(const QPoint &pos, const QColor &color) {
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

static QLabel *emptyLabel(bool selectable = false) {
    auto *label = new QLabel("-");
    label->setFixedWidth(40);
    label->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    label->setContentsMargins(0, 0, 0, 0);
    if (selectable) {
        label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    }
    return label;
}

QWidget *ImViewDockWidget::initStatusBar() {

    auto *statusBar = new QWidget(this);
    auto *layout = new QHBoxLayout();

    colLabel_ = emptyLabel();
    xLabel_ = emptyLabel();
    yLabel_ = emptyLabel();
    rLabel_ = emptyLabel();
    gLabel_ = emptyLabel();
    bLabel_ = emptyLabel();
    hLabel_ = emptyLabel();
    sLabel_ = emptyLabel();
    vLabel_ = emptyLabel();
    lLabel_ = emptyLabel();
    hexLabel_ = emptyLabel(true);
    layout->addWidget(spacing());
    layout->addWidget(new QLabel("Color: "));
    layout->addWidget(colLabel_);
    layout->addWidget(spacing());


    layout->addWidget(new QLabel("X: "));
    layout->addWidget(xLabel_);
    layout->addWidget(new QLabel("Y: "));
    layout->addWidget(yLabel_);

    layout->addWidget(spacing());

    layout->addWidget(new QLabel("R: "));
    layout->addWidget(rLabel_);
    layout->addWidget(new QLabel("G: "));
    layout->addWidget(gLabel_);
    layout->addWidget(new QLabel("B: "));
    layout->addWidget(bLabel_);

    layout->addWidget(spacing());

    layout->addWidget(new QLabel("H: "));
    layout->addWidget(hLabel_);
    layout->addWidget(new QLabel("S: "));
    layout->addWidget(sLabel_);
    layout->addWidget(new QLabel("V: "));
    layout->addWidget(vLabel_);
    layout->addWidget(new QLabel("L: "));
    layout->addWidget(lLabel_);


    layout->addWidget(spacing());

    layout->addWidget(new QLabel("Hex: "));
    layout->addWidget(hexLabel_);
    layout->addWidget(spacing());

    layout->addStretch();
    statusBar->setStyleSheet("color: grey;");
    statusBar->setLayout(layout);
    return statusBar;
}

ImViewDockWidget::~ImViewDockWidget() = default;

} // namespace nitro::ImCore
