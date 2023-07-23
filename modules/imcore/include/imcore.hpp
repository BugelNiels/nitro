#pragma once

#include "nitromodule.hpp"
#include <nodes/noderegistry.hpp>
#include <nodes/nitronodebuilder.hpp>

namespace nitro::ImCore {
class ImageViewer;
class HistogramView;

class ImCore : public NitroModule {
public:
    ImCore();

    void registerDataTypes() override;

    void registerNodes(std::shared_ptr<NodeRegistry>& registry, MainWindow *window) override;

private:
    MainWindow *window_;

    void registerInputNodes(std::shared_ptr<NodeRegistry>& registry);

    void registerOutputNodes(std::shared_ptr<NodeRegistry>& registry);

    void registerConvertNodes(std::shared_ptr<NodeRegistry>& registry);

    void registerColorNodes(std::shared_ptr<NodeRegistry>& registry);

    void registerTransformNodes(std::shared_ptr<NodeRegistry>& registry);


};

} // namespace nitro::ImCore
