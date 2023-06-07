#include "nitronodes.hpp"

std::pair<QString, std::vector<QtNodes::NodeInfo>>
nitro::NitroNodes::buildColorNodes(std::vector<RegistryItemCreator> &creators) {
    QString category = "Color";
    std::vector<QtNodes::NodeInfo> nodeInfos;

    return {category, nodeInfos};
}