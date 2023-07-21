#pragma once

#include "nitromodule.hpp"
#include "nodes/noderegistry.hpp"
#include "nodes/nitronodebuilder.hpp"

namespace nitro::Thesis {

class RenderView;

class Thesis : public NitroModule {
public:
    Thesis();

    void registerDocks(MainWindow *window) override;

    void registerNodes(NodeRegistry *registry) override;

private:
    RenderView *renderViewer_;

    void registerOutputNodes(NodeRegistry *registry, RenderView *renderViewer);

    void registerImageNodes(NodeRegistry *registry);

    void registerRestorationNodes(NodeRegistry *registry);
};

} // namespace nitro::Thesis

