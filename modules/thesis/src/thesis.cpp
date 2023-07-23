#include "thesis.hpp"

#include "gui/im3dviewer/renderdockwidget.hpp"
#include "gui/im3dviewer/renderview.hpp"

#include <gui/mainwindow.hpp>
#include <nodes/noderegistry.hpp>

#include "nodes/compression/compress.hpp"
#include "nodes/compression/decompress.hpp"
#include "nodes/compression/layerremoval.hpp"
#include "nodes/output/surfaceviewoperator.hpp"
#include "nodes/restoration/resample.hpp"

namespace nitro::Thesis {

Thesis::Thesis() = default;

void Thesis::registerNodes(std::shared_ptr<NodeRegistry> &registry, MainWindow *window) {
    window_ = window;
    registerOutputNodes(registry);
    registerImageNodes(registry);
    registerRestorationNodes(registry);
}

void Thesis::registerImageNodes(std::shared_ptr<NodeRegistry> &registry) {
    const QString category = "Compression";
    registry->registerNode(LayerRemovalOperator::creator(category));
    registry->registerNode(CompressOperator::creator(category));
    registry->registerNode(DecompressOperator::creator(category));
}

void Thesis::registerRestorationNodes(std::shared_ptr<NodeRegistry> &registry) {
    const QString category = "Restoration";
    registry->registerNode(ResampleOperator::creator(category));
}

void Thesis::registerOutputNodes(std::shared_ptr<NodeRegistry> &registry) {
    const QString category = "Output";
    registry->registerNode(SurfaceViewOperator::creator(category, window_));
}

} // namespace nitro::Thesis
