#pragma once

#include "nitromodule.hpp"
#include <nodes/nitronodebuilder.hpp>
#include <nodes/noderegistry.hpp>

namespace nitro::ImCore {
class ImageViewer;
class HistogramView;

/**
 * @brief Describes the ImCore module. This module contains a number of nodes relevant for manipulating images and numbers. Also adds support for the image data types and image viewers.
 * Any module relying on images should require this module.
 */
class ImCore : public NitroModule {
public:
    /**
     * @brief Creates the module.
     */
    ImCore();

    /**
     * @brief Registers the data types for grayscale and color images.
     */
    void registerDataTypes() override;

    /**
     * @inherit
     */
    void registerNodes(std::shared_ptr<NodeRegistry> &registry, MainWindow *window) override;

private:
    MainWindow *window_;

    void registerInputNodes(std::shared_ptr<NodeRegistry> &registry);

    void registerOutputNodes(std::shared_ptr<NodeRegistry> &registry);

    void registerConvertNodes(std::shared_ptr<NodeRegistry> &registry);

    void registerColorNodes(std::shared_ptr<NodeRegistry> &registry);

    void registerTransformNodes(std::shared_ptr<NodeRegistry> &registry);
};

} // namespace nitro::ImCore
