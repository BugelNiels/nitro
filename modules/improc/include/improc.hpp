#pragma once

#include "nitromodule.hpp"
#include <nodes/noderegistry.hpp>
#include <nodes/nitronodebuilder.hpp>

namespace nitro::ImProc {

class ImageViewer;

class ImProc : public NitroModule {
public:
    ImProc();

    void registerNodes(std::shared_ptr<NodeRegistry>& registry, MainWindow *window) override;

private:

    void registerFilterNodes(std::shared_ptr<NodeRegistry>& registry) const;

    void registerAnalysisNodes(std::shared_ptr<NodeRegistry>& registry) const;

    void registerInputNodes(std::shared_ptr<NodeRegistry>& registry) const;

    void registerQualityMetricNodes(std::shared_ptr<NodeRegistry>& registry) const;

    void registerRestorationNodes(std::shared_ptr<NodeRegistry>& registry) const;

    void registerQuantizationNodes(std::shared_ptr<NodeRegistry>& registry);
};

} // namespace nitro::ImProc
