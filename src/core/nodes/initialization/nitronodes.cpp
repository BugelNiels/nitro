#include "nitronodes.hpp"
#include "src/core/nodes/operators/output/imageviewalgorithm.hpp"
#include "src/core/nodes/operators/output/surfaceviewalgorithm.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include "nodes/datatypes/greyimagedata.hpp"
#include "nodes/datatypes/integerdata.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include "QtNodes/DataColors.hpp"


nitro::NitroNodes::NitroNodes(nitro::ImageViewer *imViewer, nitro::RenderView *surfViewer)
        : imViewer_(imViewer),
          surfViewer_(surfViewer) {
    createRegistry();

    QtNodes::DataColors::registerColor(nitro::ColorImageData::dataInfo());
    QtNodes::DataColors::registerColor(nitro::GreyImageData::dataInfo());
    QtNodes::DataColors::registerColor(nitro::IntegerData::dataInfo());
    QtNodes::DataColors::registerColor(nitro::DecimalData::dataInfo());
}

std::vector<RegistryItemCreator> nitro::NitroNodes::buildNodes() {
    std::vector<RegistryItemCreator> creators;

    categories_.emplace_back(buildInputNodes(creators));
    categories_.emplace_back(buildOutputNodes(creators));
    categories_.emplace_back(buildConverterNodes(creators));
    categories_.emplace_back(buildComparisonNodes(creators));
    categories_.emplace_back(buildFilterNodes(creators));
    categories_.emplace_back(buildColorNodes(creators));
    categories_.emplace_back(buildQuantizationNodes(creators));
    categories_.emplace_back(buildResampleNodes(creators));

    return creators;
}

void nitro::NitroNodes::createRegistry() {
    registry_ = std::make_shared<QtNodes::NodeDelegateModelRegistry>();

    auto nodes = buildNodes();

    for (auto &nodeCreator: nodes) {
        registry_->registerModel<nitro::NitroNode>(nodeCreator);
    }

    registry_->registerModel<nitro::NitroNode>();
}

const std::shared_ptr<QtNodes::NodeDelegateModelRegistry> &nitro::NitroNodes::getRegistry() const {
    return registry_;
}

const std::vector<std::pair<QString, std::vector<QtNodes::NodeInfo>>> &nitro::NitroNodes::getCategories() const {
    return categories_;
}
