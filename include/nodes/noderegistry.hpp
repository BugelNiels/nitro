#pragma once

#include "nitronodebuilder.hpp"
#include "QtNodes/NodeDelegateModelRegistry"
#include "QtNodes/DataInfo.hpp"
#include "QtNodes/NodeInfo.hpp"

// TODO: check usage
using RegistryItemPtr = std::unique_ptr<QtNodes::NodeDelegateModel>;
using RegistryItemCreator = std::function<RegistryItemPtr()>;

namespace nitro {
    class NodeRegistry {

    public:
        NodeRegistry();

        [[nodiscard]] const std::shared_ptr<QtNodes::NodeDelegateModelRegistry> &getRegistry() const;

        [[nodiscard]] std::vector<std::pair<QString, std::vector<QtNodes::NodeInfo>>> getCategories() const;

        void registerNode(const std::function<std::unique_ptr<NitroNode>()>& buildFunction);

        void registerDataType(const QtNodes::DataInfo &info);

    private:
        std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registry_;
        std::map<QString, std::vector<QtNodes::NodeInfo>> categories_;
    };
}

