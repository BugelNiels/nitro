#pragma once

#include "nitromodule.hpp"
#include <nodes/noderegistry.hpp>
#include <nodes/nitronodebuilder.hpp>

namespace nitro::Thesis {

class RenderView;

class Thesis : public NitroModule {
public:
    Thesis();

    void registerNodes(NodeRegistry *registry, MainWindow *window) override;

private:
    MainWindow *window_;

    void registerOutputNodes(NodeRegistry *registry);

    void registerImageNodes(NodeRegistry *registry);

    void registerRestorationNodes(NodeRegistry *registry);

};

} // namespace nitro::Thesis

