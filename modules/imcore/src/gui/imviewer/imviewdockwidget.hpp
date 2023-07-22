#pragma once

#include <QDockWidget>
#include <QStatusBar>
#include <QLabel>

namespace nitro {
class MainWindow;
}

namespace nitro::ImCore {


class ImageViewer;

class ImViewDockWidget : public QDockWidget {
public:

    explicit ImViewDockWidget(ImageViewer *imageViewer, MainWindow *window);

    ~ImViewDockWidget() override;

private:
    ImageViewer *imageViewer_;

    QWidget *initTitleBarWidget(MainWindow *window) const;
};

} // namespace nitro::ImCore
