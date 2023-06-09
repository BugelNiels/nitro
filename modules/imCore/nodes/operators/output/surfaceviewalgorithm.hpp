#pragma once

#include "include/nodes/nodeoperator.hpp"
#include "modules/imCore/gui/surfacevis/renderview.hpp"

namespace nitro {

    class SurfaceViewAlgorithm : public NodeOperator {
    public:
        explicit SurfaceViewAlgorithm(RenderView *surfViewer);

        void execute(NodePorts &nodePorts, const std::map<QString, int> &options) const override;

    private:
        RenderView *surfViewer_;
    };
}