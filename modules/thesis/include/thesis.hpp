#pragma once

#include "nitromodule.hpp"
#include "nodes/noderegistry.hpp"
#include "nodes/nitronodebuilder.hpp"

namespace nitro::Thesis {

class RenderView;

class ThesisModule : public NitroModule {
public:
    ThesisModule();

    void registerDocks(MainWindow *window) override;

    void registerNodes(NodeRegistry *registry) override;

private:

    void registerOutputNodes(NodeRegistry *registry, RenderView *renderViewer);

    void registerImageNodes(NodeRegistry *registry);

    RenderView *renderViewer_;

    void registerRestorationNodes(NodeRegistry *registry);
};

} // namespace nitro::Thesis

