#pragma once

#include <QDockWidget>
#include <QLabel>
#include <QStatusBar>

namespace nitro {
class MainWindow;
}

namespace nitro::ImCore {

class HistogramView;

class HistogramViewerDockWidget : public QDockWidget {
public:
    explicit HistogramViewerDockWidget(HistogramView *histViewer, MainWindow *window);

    ~HistogramViewerDockWidget() override;

private:
    HistogramView *histViewer_;

    QWidget *initTitleBarWidget(MainWindow *window);
};

} // namespace nitro::ImCore
