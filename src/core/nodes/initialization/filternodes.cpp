#include "nitronodes.hpp"


std::pair<QString, std::vector<QtNodes::NodeInfo>>
nitro::NitroNodes::buildFilterNodes(std::vector<RegistryItemCreator> &creators) {
    QString category = "Filter";
    std::vector<QtNodes::NodeInfo> nodeInfos;

    return {category, nodeInfos};
}