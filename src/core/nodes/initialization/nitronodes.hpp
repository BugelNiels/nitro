#pragma once

#include "src/core/nodes/nitronodebuilder.hpp"
#include "src/core/nodes/nitronode.hpp"
#include "QtNodes/NodeDelegateModelRegistry"
#include "ui/imgviewer/imgviewer.hpp"
#include "ui/surfacevis/renderview.hpp"


// TODO: check usage
using RegistryItemPtr = std::unique_ptr<QtNodes::NodeDelegateModel>;
using RegistryItemCreator = std::function<RegistryItemPtr()>;

namespace nitro {
    class NitroNodes {

    public:
        NitroNodes(nitro::ImageViewer *pViewer, nitro::RenderView *pView);

        [[nodiscard]] const std::shared_ptr<QtNodes::NodeDelegateModelRegistry> &getRegistry() const;

        [[nodiscard]] const std::vector<std::pair<QString, std::vector<QtNodes::NodeInfo>>> &getCategories() const;

    private:
        std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registry_;
        std::vector<std::pair<QString, std::vector<QtNodes::NodeInfo>>> categories_;

        ImageViewer *imViewer_;
        RenderView *surfViewer_;

        void createRegistry();

        std::vector<RegistryItemCreator> buildNodes();

        std::pair<QString, std::vector<QtNodes::NodeInfo>> buildInputNodes(std::vector<RegistryItemCreator> &creators);
        std::pair<QString, std::vector<QtNodes::NodeInfo>> buildOutputNodes(std::vector<RegistryItemCreator> &creators);
        std::pair<QString, std::vector<QtNodes::NodeInfo>> buildConverterNodes(std::vector<RegistryItemCreator> &creators);
        std::pair<QString, std::vector<QtNodes::NodeInfo>> buildComparisonNodes(std::vector<RegistryItemCreator> &creators);
        std::pair<QString, std::vector<QtNodes::NodeInfo>> buildFilterNodes(std::vector<RegistryItemCreator> &creators);
        std::pair<QString, std::vector<QtNodes::NodeInfo>> buildColorNodes(std::vector<RegistryItemCreator> &creators);
        std::pair<QString, std::vector<QtNodes::NodeInfo>> buildQuantizationNodes(std::vector<RegistryItemCreator> &creators);
        std::pair<QString, std::vector<QtNodes::NodeInfo>> buildResampleNodes(std::vector<RegistryItemCreator> &creators);

    };
}

