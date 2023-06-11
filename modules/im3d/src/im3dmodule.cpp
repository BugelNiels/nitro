#include "im3dmodule.hpp"

#include "gui/im3dviewer/renderview.hpp"
#include "gui/im3dviewer/renderdockwidget.hpp"

#include "nodes/noderegistry.hpp"
#include "gui/mainwindow.hpp"

#include "nodes/operators/image/layerremoval.hpp"
#include "nodes/operators/image/resample.hpp"
#include "nodes/operators/output/surfaceviewalgorithm.hpp"

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

    void Im3DModule::registerImageNodes(NodeRegistry* registry) {
        const QString category = "Image";
        registry->registerNode(LayerRemovalOperator::creator(category));
        registry->registerNode(ResampleOperator::creator(category));
    }

    void Im3DModule::registerOutputNodes(NodeRegistry* registry, RenderView* renderViewer) {
        const QString category = "Output";
        registry->registerNode(SurfaceViewAlgorithm::creator(category, renderViewer));
    }

} // imCore
