#pragma once

#include "src/core/nodes/operators/nodeoperator.hpp"
#include "surfacevis/renderview.hpp"

namespace nitro {

    class SurfaceViewAlgorithm : public NodeOperator {
    public:
        explicit SurfaceViewAlgorithm(RenderView *surfViewer);

        void execute(NodePorts &nodePorts, const std::map<QString, int> &options) const override;

    private:
        RenderView *surfViewer_;
    };
}