#pragma once

#include "nitronodebuilder.hpp"
#include "QtNodes/NodeDelegateModelRegistry"
#include "QtNodes/NodeInfo.hpp"

namespace nitro {
    class NodeRegistry {

    public:
        NodeRegistry();

        [[nodiscard]] const std::shared_ptr<QtNodes::NodeDelegateModelRegistry> &getRegistry() const;

        [[nodiscard]] std::vector<std::pair<QString, std::vector<QtNodes::NodeInfo>>> getCategories() const;

        void registerNode(const std::function<std::unique_ptr<NitroNode>()> &buildFunction);

    private:
        int catIdx_ = 0;
        std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registry_;
        std::map<QString, std::vector<QtNodes::NodeInfo>> categories_;
        std::map<QString, int> categoryOrder_;
    };
}

