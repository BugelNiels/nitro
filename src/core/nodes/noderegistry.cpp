#include <nodes/noderegistry.hpp>

#include <nodes/nitronode.hpp>

nitro::NodeRegistry::NodeRegistry() {
    registry_ = std::make_shared<QtNodes::NodeDelegateModelRegistry>();
}

const std::shared_ptr<QtNodes::NodeDelegateModelRegistry> &nitro::NodeRegistry::getRegistry()
        const {
    return registry_;
}

std::vector<std::pair<QString, std::vector<QtNodes::NodeInfo>>> nitro::NodeRegistry::getCategories()
        const {
    std::vector<std::pair<QString, std::vector<QtNodes::NodeInfo>>> categories;
    categories.resize(categoryOrder_.size());

    for (auto &item: categoryOrder_) {
        QString cat = item.first;
        categories[item.second] = {cat, categories_.at(cat)};
    }
    return categories;
}

void nitro::NodeRegistry::registerNode(
        const std::function<std::unique_ptr<NitroNode>()> &buildFunction) {
    QtNodes::NodeInfo info = buildFunction()->getInfo();
    categories_[info.getCategory()].emplace_back(info);
    if (categoryOrder_.count(info.getCategory()) == 0) {
        categoryOrder_[info.getCategory()] = catIdx_;
        catIdx_++;
    }
    registry_->registerModel<nitro::NitroNode>(buildFunction);
}
