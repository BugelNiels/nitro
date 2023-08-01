#include "histogramdockwidget.hpp"
#include "histogramviewer.hpp"

#include <gui/mainwindow.hpp>

namespace nitro::ImCore {

HistogramViewerDockWidget::HistogramViewerDockWidget(HistogramView *histViewer, MainWindow *window)
    : NitroDockWidget("Histogram Viewer", window),
      histViewer_(histViewer) {
    setWindowTitle("Histogram Viewer");

    setWidget(histViewer_);
    setIcon(":/icons/hist_viewer.png");
}

HistogramViewerDockWidget::~HistogramViewerDockWidget() = default;

} // namespace nitro::ImCore
