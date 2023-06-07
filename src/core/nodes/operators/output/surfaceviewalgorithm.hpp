#pragma once

#include "src/core/nodes/operators/nodeoperator.hpp"
#include "ui/surfacevis/renderview.hpp"

namespace nitro {

    class SurfaceViewAlgorithm : public NodeOperator {
    public:
        explicit SurfaceViewAlgorithm(RenderView *surfViewer);

        void execute(NodePorts &nodePorts, const std::map<QString, QString> &options) const override;

    private:
        RenderView *surfViewer_;
    };
}