#include "nitronodes.hpp"

std::pair<QString, std::vector<QtNodes::NodeInfo>>
nitro::NitroNodes::buildComparisonNodes(std::vector<RegistryItemCreator> &creators) {
    QString category = "Comparison";
    std::vector<QtNodes::NodeInfo> nodeInfos;

    return {category, nodeInfos};
}