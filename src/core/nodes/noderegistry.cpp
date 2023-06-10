#include "include/nodes/noderegistry.hpp"

#include "QtNodes/DataColors.hpp"
#include "include/nodes/nitronode.hpp"


nitro::NodeRegistry::NodeRegistry() {
    registry_ = std::make_shared<QtNodes::NodeDelegateModelRegistry>();
}

const std::shared_ptr<QtNodes::NodeDelegateModelRegistry> &nitro::NodeRegistry::getRegistry() const {
    return registry_;
}

std::vector<std::pair<QString, std::vector<QtNodes::NodeInfo>>> nitro::NodeRegistry::getCategories() const {
    std::vector<std::pair<QString, std::vector<QtNodes::NodeInfo>>> categories;
    for (auto &key: categories_) {
        categories.emplace_back(key);
    }
    return categories;
}

void nitro::NodeRegistry::registerNode(const std::function<std::unique_ptr<NitroNode>()>& buildFunction) {
    QtNodes::NodeInfo info = buildFunction()->getInfo();
    categories_[info.getCategory()].emplace_back(info);
    RegistryItemCreator creator = buildFunction;
    registry_->registerModel<nitro::NitroNode>(creator);
}

void nitro::NodeRegistry::registerDataType(const QtNodes::DataInfo &info) {
    QtNodes::DataColors::registerColor(info);
}
