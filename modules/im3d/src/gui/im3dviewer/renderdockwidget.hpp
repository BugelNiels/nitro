#pragma once

#include <QDockWidget>

namespace nitro {
    class MainWindow;
    class RenderView;

    class RenderDockWidget : public QDockWidget {
    public:

        explicit RenderDockWidget(RenderView *renderView, MainWindow* window);

        ~RenderDockWidget() override;

    private:

        RenderView *renderView_;
    };

} // nitro