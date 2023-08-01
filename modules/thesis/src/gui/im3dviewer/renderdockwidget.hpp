#pragma once

#include "gui/nitrodockwidget.hpp"

namespace nitro {

class MainWindow;

namespace Thesis {

class RenderView;

class RenderDockWidget : public NitroDockWidget {
public:
    explicit RenderDockWidget(RenderView *renderView, MainWindow *window);

    ~RenderDockWidget() override;

private:
    RenderView *renderView_;
};

} // namespace Thesis
} // namespace nitro
