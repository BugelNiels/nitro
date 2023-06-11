#include "im3dmodule.hpp"

#include "gui/im3dviewer/renderview.hpp"
#include "gui/im3dviewer/renderdockwidget.hpp"

#include "nodes/noderegistry.hpp"
#include "gui/mainwindow.hpp"

namespace nitro::Im3D {

    Im3DModule::Im3DModule() {
        renderViewer_ = new RenderView();
    }

    void Im3DModule::registerNodes(NodeRegistry *registry) {
        registerOutputNodes(registry, renderViewer_);
        registerImageNodes(registry);
    }

    void Im3DModule::registerDataTypes(NodeRegistry *registry) {
    }

    void Im3DModule::registerDocks(nitro::MainWindow *window) {
        auto im3DViewDock = new RenderDockWidget(renderViewer_, window);
        window->registerDock(im3DViewDock);
    }

} // imCore