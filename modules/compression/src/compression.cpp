#include "compression.hpp"

#include "nodes/noderegistry.hpp"
#include "gui/mainwindow.hpp"
#include "nodes/compression/zliboperator.hpp"
#include "nodes/compression/pngoperator.hpp"
#include "nodes/compression/jpegoperator.hpp"


namespace nitro::Compression {

Compression::Compression() = default;

void Compression::registerNodes(NodeRegistry *registry) {
    registerCompressionNodes(registry);
}

void Compression::registerCompressionNodes(NodeRegistry *registry) {
    const QString category = "Compression";
    registry->registerNode(ZLibOperator::creator(category));
    registry->registerNode(PngOperator::creator(category));
    registry->registerNode(JpegOperator::creator(category));
}

} // namespace nitro::Compression
