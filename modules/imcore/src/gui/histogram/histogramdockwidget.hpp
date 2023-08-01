#pragma once

#include "gui/nitrodockwidget.hpp"
#include <QLabel>
#include <QStatusBar>

namespace nitro {
class MainWindow;
}

namespace nitro::ImCore {

class HistogramView;

class HistogramViewerDockWidget : public NitroDockWidget {
public:
    explicit HistogramViewerDockWidget(HistogramView *histViewer, MainWindow *window);

    ~HistogramViewerDockWidget() override;

private:
    HistogramView *histViewer_;
};

} // namespace nitro::ImCore
