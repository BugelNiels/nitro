#pragma once

#include "nitromodule.hpp"
#include <nodes/nitronodebuilder.hpp>
#include <nodes/noderegistry.hpp>

namespace nitro::Compression {

/**
 * @brief Describes the Compression module. Contains numerous nodes that allow for the compression of images.
 */
class Compression : public NitroModule {
public:
    /**
     * @brief Creates the module.
     */
    Compression();

    /**
     * @inherit
     */
    void registerNodes(std::shared_ptr<NodeRegistry> &registry, MainWindow *window) override;

private:
    void registerCompressionNodes(std::shared_ptr<NodeRegistry> &registry);
};

} // namespace nitro::Compression
