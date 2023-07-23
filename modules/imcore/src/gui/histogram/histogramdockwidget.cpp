#include "histogramdockwidget.hpp"
#include "histogramviewer.hpp"

#include <gui/mainwindow.hpp>
#include <QTabBar>
#include <QHBoxLayout>

namespace nitro::ImCore {

const int SPACING = 10;

static QWidget *spacing() {
    auto label = new QLabel("|");
    label->setContentsMargins(SPACING, 0, SPACING, 0);
    return label;
}

HistogramViewerDockWidget::HistogramViewerDockWidget(HistogramView *histViewer, MainWindow *window)
        : QDockWidget(window), histViewer_(histViewer) {
    setWindowTitle("Histogram Viewer");

    QWidget *imViewTitleWrapper = initTitleBarWidget(window);

    setTitleBarWidget(imViewTitleWrapper);

    setWidget(histViewer_);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setFeatures(features() & ~QDockWidget::DockWidgetClosable);
}

QWidget *HistogramViewerDockWidget::initTitleBarWidget(MainWindow *window) {
    auto imViewTitleWrapper = new QWidget();
    auto imHLayout = new QHBoxLayout();

    imHLayout->addWidget(window->buildDockIcon(":/icons/hist_viewer.png"));
    imViewTitleWrapper->setLayout(imHLayout);
    return imViewTitleWrapper;
}

HistogramViewerDockWidget::~HistogramViewerDockWidget() = default;

} // namespace nitro::ImCore
