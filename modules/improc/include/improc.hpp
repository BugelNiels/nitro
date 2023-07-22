#pragma once

#include "nitromodule.hpp"
#include <nodes/noderegistry.hpp>
#include <nodes/nitronodebuilder.hpp>

namespace nitro::ImProc {

class ImageViewer;

class ImProc : public NitroModule {
public:
    ImProc();

    void registerNodes(NodeRegistry *registry, MainWindow *window) override;

private:

    void registerFilterNodes(NodeRegistry *registry) const;

    void registerAnalysisNodes(NodeRegistry *registry) const;

    void registerInputNodes(NodeRegistry *registry) const;

    void registerQualityMetricNodes(NodeRegistry *registry) const;

    void registerRestorationNodes(NodeRegistry *registry) const;

    void registerQuantizationNodes(NodeRegistry *registry);
};

} // namespace nitro::ImProc
