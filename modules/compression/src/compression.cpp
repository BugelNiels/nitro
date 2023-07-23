#include "compression.hpp"

#include <nodes/noderegistry.hpp>
#include <gui/mainwindow.hpp>
#include "nodes/compression/zliboperator.hpp"
#include "nodes/compression/pngoperator.hpp"
#include "nodes/compression/jpegoperator.hpp"


namespace nitro::Compression {

Compression::Compression() = default;

void Compression::registerNodes(std::shared_ptr<NodeRegistry>& registry, MainWindow *window) {
    Q_UNUSED(window);
    registerCompressionNodes(registry);
}

void Compression::registerCompressionNodes(std::shared_ptr<NodeRegistry>& registry) {
    const QString category = "Compression";
    registry->registerNode(ZLibOperator::creator(category));
    registry->registerNode(PngOperator::creator(category));
    registry->registerNode(JpegOperator::creator(category));
}

} // namespace nitro::Compression
