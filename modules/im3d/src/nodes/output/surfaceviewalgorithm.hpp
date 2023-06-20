#pragma once

#include "nodes/nodeoperator.hpp"
#include "gui/im3dviewer/renderview.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    class SurfaceViewAlgorithm : public NodeOperator {

    public:

        static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category, RenderView *renderViewer);

        explicit SurfaceViewAlgorithm(RenderView *surfViewer);

        void execute(NodePorts &nodePorts, const std::map<QString, int> &options) override;

    private:
        RenderView *surfViewer_;
        cv::Mat currentImg_;
    };
}
