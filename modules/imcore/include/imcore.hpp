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

    void registerNodes(NodeRegistry *registry, MainWindow *window) override;

private:
    MainWindow *window_;

    void registerInputNodes(NodeRegistry *registry);

    void registerOutputNodes(NodeRegistry *registry);

    void registerConvertNodes(NodeRegistry *registry);

    void registerColorNodes(NodeRegistry *registry);

    void registerTransformNodes(NodeRegistry *registry);


};

} // namespace nitro::ImCore
