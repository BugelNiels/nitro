#include "thesis.hpp"

#include "gui/im3dviewer/renderview.hpp"
#include "gui/im3dviewer/renderdockwidget.hpp"

#include "nodes/noderegistry.hpp"
#include "gui/mainwindow.hpp"

#include "nodes/output/surfaceviewoperator.hpp"
#include "nodes/compression/layerremoval.hpp"
#include "nodes/restoration/resample.hpp"
#include "nodes/compression/compress.hpp"
#include "nodes/compression/decompress.hpp"

namespace nitro::Thesis {
ThesisModule::ThesisModule() {
    renderViewer_ = new RenderView();
}

void ThesisModule::registerNodes(NodeRegistry *registry) {
    registerOutputNodes(registry, renderViewer_);
    registerImageNodes(registry);
    registerRestorationNodes(registry);
}

void ThesisModule::registerDocks(nitro::MainWindow *window) {
    auto im3DViewDock = new RenderDockWidget(renderViewer_, window);
    window->registerDock(im3DViewDock);
}

void ThesisModule::registerImageNodes(NodeRegistry *registry) {
    const QString category = "Compression";
    registry->registerNode(LayerRemovalOperator::creator(category));
    registry->registerNode(CompressOperator::creator(category));
    registry->registerNode(DecompressOperator::creator(category));
}

void ThesisModule::registerRestorationNodes(NodeRegistry *registry) {
    const QString category = "Restoration";
    registry->registerNode(ResampleOperator::creator(category));
}

void ThesisModule::registerOutputNodes(NodeRegistry *registry, RenderView *renderViewer) {
    const QString category = "Output";
    registry->registerNode(SurfaceViewOperator::creator(category, renderViewer));
}
} // namespace nitro::Thesis
