#pragma once

#include "nitromodule.hpp"
#include <nodes/nitronodebuilder.hpp>
#include <nodes/noderegistry.hpp>

namespace nitro::Thesis {

class RenderView;

/**
 * @brief Describes the Thesis module. This module contains the node and functionality used throughout Niels' Master's thesis.
 * Includes the 3D image viewer.
 */
class Thesis : public NitroModule {
public:
    /**
     * @brief Creates the module.
     */
    Thesis();

    /**
     * @inherit
     */
    void registerNodes(std::shared_ptr<NodeRegistry> &registry, MainWindow *window) override;

private:
    MainWindow *window_ = nullptr;

    void registerOutputNodes(std::shared_ptr<NodeRegistry> &registry);

    void registerImageNodes(std::shared_ptr<NodeRegistry> &registry);

    void registerRestorationNodes(std::shared_ptr<NodeRegistry> &registry);
};

} // namespace nitro::Thesis
