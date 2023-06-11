#pragma once

#include <opencv2/imgproc.hpp>
#include "nodes/nodeoperator.hpp"

namespace nitro {

    class SeparateOperator : public NodeOperator {

        void execute(NodePorts &nodePorts, const std::map<QString, int> &options) const override;

    };

} // nitro
