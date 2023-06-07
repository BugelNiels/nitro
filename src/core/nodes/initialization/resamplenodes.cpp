#include "nitronodes.hpp"

std::pair<QString, std::vector<QtNodes::NodeInfo>>
nitro::NitroNodes::buildResampleNodes(std::vector<RegistryItemCreator> &creators) {
    QString category = "Resampling";
    std::vector<QtNodes::NodeInfo> nodeInfos;

    return {category, nodeInfos};
}