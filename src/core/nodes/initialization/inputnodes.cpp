#include "nitronodes.hpp"

std::pair<QString, std::vector<QtNodes::NodeInfo>>
nitro::NitroNodes::buildInputNodes(std::vector<RegistryItemCreator> &creators) {
    QString category = "Input";
    std::vector<QtNodes::NodeInfo> nodeInfos;

    // ------ Image Source Node ------
    auto imSourceNode = [category]() {
        nitro::NitroNodeBuilder builder("Image Source", "ImageSource", category);
        auto node = builder.
                withLoadedOutputImage("image")->
                withIcon(":/icons/nodes/image_source.png")->
                withNodeColor({121, 70, 29})->
                build();
        return node;
    };
    nodeInfos.emplace_back(imSourceNode()->getInfo());
    creators.emplace_back(std::move(imSourceNode));

    // ------ Decimal Source Node ------
    auto decimalSourceNode = [category]() {
        nitro::NitroNodeBuilder builder("Value", "ValueSource", category);
        auto node = builder.
                withSourcedOutputValue("Value")->
                withIcon(":/icons/nodes/number.png")->
                withNodeColor({131, 49, 74})->
                build();
        return node;
    };
    nodeInfos.emplace_back(decimalSourceNode()->getInfo());
    creators.emplace_back(std::move(decimalSourceNode));

    // ------ Integer Source Node ------
    auto intSourceNode = [category]() {
        nitro::NitroNodeBuilder builder("Integer", "IntegerSource", category);
        auto node = builder.
                withSourcedOutputInteger("Integer")->
                withIcon(":/icons/nodes/number.png")->
                withNodeColor({131, 49, 74})->
                build();
        return node;
    };
    nodeInfos.emplace_back(intSourceNode()->getInfo());
    creators.emplace_back(std::move(intSourceNode));

    return {category, nodeInfos};
}