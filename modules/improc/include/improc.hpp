#pragma once

#include "nitromodule.hpp"
#include <nodes/nitronodebuilder.hpp>
#include <nodes/noderegistry.hpp>

namespace nitro::ImProc {

class ImageViewer;

/**
 * @brief Describes the ImProc module. This module contains a number of nodes used for image processing, ranging from various filters to restoration and quantization nodes.
 */
class ImProc : public NitroModule {
public:
    /**
     * @brief Creates the module.
     */
    ImProc();

    /**
     * @inherit
     */
    void registerNodes(std::shared_ptr<NodeRegistry> &registry, MainWindow *window) override;

private:
    void registerFilterNodes(std::shared_ptr<NodeRegistry> &registry) const;

    void registerAnalysisNodes(std::shared_ptr<NodeRegistry> &registry) const;

    void registerInputNodes(std::shared_ptr<NodeRegistry> &registry) const;

    void registerQualityMetricNodes(std::shared_ptr<NodeRegistry> &registry) const;

    void registerRestorationNodes(std::shared_ptr<NodeRegistry> &registry) const;

    void registerQuantizationNodes(std::shared_ptr<NodeRegistry> &registry);
};

} // namespace nitro::ImProc
