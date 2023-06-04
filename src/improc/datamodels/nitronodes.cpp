#include "nitronodes.hpp"
#include "nodeeditor/style/nodecolors.hpp"
#include "core/algorithms/util/threshold.hpp"
#include "nodeeditor/style/datacolors.hpp"
#include "datamodels/datatypes/imagedata.hpp"
#include "datamodels/datatypes/integerdata.hpp"
#include "datamodels/datatypes/decimaldata.hpp"


nitro::NitroNodes::NitroNodes() {
    createRegistry();

    nitro::DataColors::registerColor(nitro::ImageData::dataInfo());
    nitro::DataColors::registerColor(nitro::IntegerData::dataInfo());
    nitro::DataColors::registerColor(nitro::DecimalData::dataInfo());
}

std::pair<QString, std::vector<nitro::NodeInfo>>
nitro::NitroNodes::buildInputNodes(std::vector<RegistryItemCreator> &creators) {
    QString category = "Input";
    std::vector<nitro::NodeInfo> nodeInfos;

    return {category, nodeInfos};
}

std::pair<QString, std::vector<nitro::NodeInfo>>
nitro::NitroNodes::buildOutputNodes(std::vector<RegistryItemCreator> &creators) {
    QString category = "Output";
    std::vector<nitro::NodeInfo> nodeInfos;

    return {category, nodeInfos};
}

std::pair<QString, std::vector<nitro::NodeInfo>>
nitro::NitroNodes::buildConverterNodes(std::vector<RegistryItemCreator> &creators) {
    QString category = "Converter";
    std::vector<nitro::NodeInfo> nodeInfos;

    // ------ Threshold Node ------
    auto thresholdNode = [category]() {
        nitro::NitroNodeBuilder builder("Threshold", "threshold", category,
                                        std::make_unique<nitro::operations::ThresholdAlgorithm>());
        auto node = builder.
                withInputImage("image")->
                withInputInteger("threshold", 128, 0, 255)->
                withOutputImage("image")->
                withIcon(":/icons/nodes/threshold.png")->
                withNodeColor({43, 101, 43})->
                build();
        return node;
    };
    nodeInfos.emplace_back(thresholdNode()->getInfo());
    creators.emplace_back(std::move(thresholdNode));
    // --------------------------


    return {category, nodeInfos};
}

std::pair<QString, std::vector<nitro::NodeInfo>>
nitro::NitroNodes::buildComparisonNodes(std::vector<RegistryItemCreator> &creators) {
    QString category = "Comparison";
    std::vector<nitro::NodeInfo> nodeInfos;

    return {category, nodeInfos};
}

std::pair<QString, std::vector<nitro::NodeInfo>>
nitro::NitroNodes::buildFilterNodes(std::vector<RegistryItemCreator> &creators) {
    QString category = "Filter";
    std::vector<nitro::NodeInfo> nodeInfos;

    return {category, nodeInfos};
}

std::pair<QString, std::vector<nitro::NodeInfo>>
nitro::NitroNodes::buildColorNodes(std::vector<RegistryItemCreator> &creators) {
    QString category = "Color";
    std::vector<nitro::NodeInfo> nodeInfos;

    return {category, nodeInfos};
}

std::pair<QString, std::vector<nitro::NodeInfo>>
nitro::NitroNodes::buildQuantizationNodes(std::vector<RegistryItemCreator> &creators) {
    QString category = "Quantization";
    std::vector<nitro::NodeInfo> nodeInfos;

    return {category, nodeInfos};
}

std::pair<QString, std::vector<nitro::NodeInfo>>
nitro::NitroNodes::buildResampleNodes(std::vector<RegistryItemCreator> &creators) {
    QString category = "Resampling";
    std::vector<nitro::NodeInfo> nodeInfos;

    return {category, nodeInfos};
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

const std::vector<std::pair<QString, std::vector<nitro::NodeInfo>>> &nitro::NitroNodes::getCategories() const {
    return categories_;
}
