#pragma once

#include "gui/nitrodockwidget.hpp"
#include <QDockWidget>
#include <QLabel>
#include <QStatusBar>

namespace nitro {
class MainWindow;
}

namespace nitro::ImCore {

class ImageViewer;

class ImViewDockWidget : public NitroDockWidget {
public:
    explicit ImViewDockWidget(ImageViewer *imageViewer, MainWindow *window);

    ~ImViewDockWidget() override;

private:
    ImageViewer *imageViewer_;
    void initTitleBar(const MainWindow *window);
};

} // namespace nitro::ImCore
