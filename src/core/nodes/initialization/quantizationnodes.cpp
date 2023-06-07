#include "nitronodes.hpp"



std::pair<QString, std::vector<QtNodes::NodeInfo>>
nitro::NitroNodes::buildQuantizationNodes(std::vector<RegistryItemCreator> &creators) {
    QString category = "Quantization";
    std::vector<QtNodes::NodeInfo> nodeInfos;

    return {category, nodeInfos};
}
