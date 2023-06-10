#pragma once

#include <QDockWidget>

namespace nitro {
    class MainWindow;

    class ImageViewer;

    class ImViewDockWidget : public QDockWidget {
    public:

        explicit ImViewDockWidget(ImageViewer *imageViewer, MainWindow *window);

        ~ImViewDockWidget() override;

    private:

        ImageViewer *imageViewer_;
    };

} // nitro