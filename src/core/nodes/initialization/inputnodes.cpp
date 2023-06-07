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

    return {category, nodeInfos};
}