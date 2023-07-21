#pragma once

#include "nitromodule.hpp"
#include "nodes/noderegistry.hpp"
#include "nodes/nitronodebuilder.hpp"

namespace nitro::ImCore {
class ImageViewer;

class ImCore : public NitroModule {
public:
    ImCore();

    void registerDataTypes(NodeRegistry *registry) override;

    void registerDocks(MainWindow *window) override;

    void registerNodes(NodeRegistry *registry) override;

private:
    ImageViewer *imageViewer_;

    void registerInputNodes(NodeRegistry *registry);

    void registerOutputNodes(NodeRegistry *registry, ImageViewer *imageViewer);

    void registerConvertNodes(NodeRegistry *registry);

    void registerColorNodes(NodeRegistry *registry);

    void registerTransformNodes(NodeRegistry *registry);
};

} // namespace nitro::ImCore
