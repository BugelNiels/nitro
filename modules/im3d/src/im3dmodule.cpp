#include "im3dmodule.hpp"

#include "gui/im3dviewer/renderview.hpp"
#include "gui/im3dviewer/renderdockwidget.hpp"

#include "nodes/noderegistry.hpp"
#include "gui/mainwindow.hpp"

#include "nodes/output/surfaceviewoperator.hpp"
#include "nodes/compression/layerremoval.hpp"
#include "nodes/restoration/resample.hpp"

namespace nitro::Im3D {

    Im3DModule::Im3DModule() {
        renderViewer_ = new RenderView();
    }

    void Im3DModule::registerNodes(NodeRegistry *registry) {
        registerOutputNodes(registry, renderViewer_);
        registerImageNodes(registry);
        registerRestorationNodes(registry);
    }

    void Im3DModule::registerDocks(nitro::MainWindow *window) {
        auto im3DViewDock = new RenderDockWidget(renderViewer_, window);
        window->registerDock(im3DViewDock);
    }

    void Im3DModule::registerImageNodes(NodeRegistry* registry) {
        const QString category = "Compression";
        registry->registerNode(LayerRemovalOperator::creator(category));
    }

    void Im3DModule::registerRestorationNodes(NodeRegistry* registry) {
        const QString category = "Restoration";
        registry->registerNode(ResampleOperator::creator(category));
    }

    void Im3DModule::registerOutputNodes(NodeRegistry* registry, RenderView* renderViewer) {
        const QString category = "Output";
        registry->registerNode(SurfaceViewOperator::creator(category, renderViewer));
    }

} // imCore
