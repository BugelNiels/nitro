#include "nitronodes.hpp"
#include "nodes/operators/converter/threshold.hpp"

std::pair<QString, std::vector<QtNodes::NodeInfo>>
nitro::NitroNodes::buildConverterNodes(std::vector<RegistryItemCreator> &creators) {
    QString category = "Converter";
    std::vector<QtNodes::NodeInfo> nodeInfos;

    // ------ Threshold Node ------
    auto thresholdNode = [category]() {
        nitro::NitroNodeBuilder builder("Threshold", "threshold", category,
                                        std::make_unique<nitro::ThresholdAlgorithm>());
        auto node = builder.
                withIcon(":/icons/nodes/threshold.png")->
                withNodeColor({43, 101, 43})->
                withDropDown("Mode", {"<=", ">="})->
                withCheckBox("check")->
                withInputGreyImage("Image")->
                withInputInteger("Threshold", 128, 0, 255)->
                withOutputGreyImage("Image")->
                withSourcedOutputInteger("test")->
                build();

        return node;
    };
    nodeInfos.emplace_back(thresholdNode()->getInfo());
    creators.emplace_back(std::move(thresholdNode));


    return {category, nodeInfos};
}