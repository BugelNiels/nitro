#pragma once

#include "nitronodebuilder.hpp"
#include "nitronode.hpp"
#include "QtNodes/NodeDelegateModelRegistry"


// TODO: check usage
using RegistryItemPtr = std::unique_ptr<QtNodes::NodeDelegateModel>;
using RegistryItemCreator = std::function<RegistryItemPtr()>;

namespace nitro {
    class NitroNodes {

    public:
        NitroNodes();

        [[nodiscard]] const std::shared_ptr<QtNodes::NodeDelegateModelRegistry> &getRegistry() const;

        [[nodiscard]] const std::vector<std::pair<QString, std::vector<NodeInfo>>> &getCategories() const;

    private:
        std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registry_;
        std::vector<std::pair<QString, std::vector<NodeInfo>>> categories_;

        void createRegistry();

        std::vector<RegistryItemCreator> buildNodes();

        std::pair<QString, std::vector<NodeInfo>> buildInputNodes(std::vector<RegistryItemCreator> &creators);
        std::pair<QString, std::vector<NodeInfo>> buildOutputNodes(std::vector<RegistryItemCreator> &creators);
        std::pair<QString, std::vector<NodeInfo>> buildConverterNodes(std::vector<RegistryItemCreator> &creators);
        std::pair<QString, std::vector<NodeInfo>> buildComparisonNodes(std::vector<RegistryItemCreator> &creators);
        std::pair<QString, std::vector<NodeInfo>> buildFilterNodes(std::vector<RegistryItemCreator> &creators);
        std::pair<QString, std::vector<NodeInfo>> buildColorNodes(std::vector<RegistryItemCreator> &creators);
        std::pair<QString, std::vector<NodeInfo>> buildQuantizationNodes(std::vector<RegistryItemCreator> &creators);
        std::pair<QString, std::vector<NodeInfo>> buildResampleNodes(std::vector<RegistryItemCreator> &creators);
    };
}

