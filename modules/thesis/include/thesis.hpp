#pragma once

#include "nitromodule.hpp"
#include <nodes/noderegistry.hpp>
#include <nodes/nitronodebuilder.hpp>

namespace nitro::Thesis {

class RenderView;

class Thesis : public NitroModule {
public:
    Thesis();

    void registerNodes(std::shared_ptr<NodeRegistry>& registry, MainWindow *window) override;

private:
    MainWindow *window_;

    void registerOutputNodes(std::shared_ptr<NodeRegistry>& registry);

    void registerImageNodes(std::shared_ptr<NodeRegistry>& registry);

    void registerRestorationNodes(std::shared_ptr<NodeRegistry>& registry);

};

} // namespace nitro::Thesis

