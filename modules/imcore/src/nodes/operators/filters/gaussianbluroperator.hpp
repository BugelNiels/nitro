#pragma once

#include "nodes/nodeoperator.hpp"

namespace nitro {

    class GaussianBlurOperator : public NodeOperator {

        void execute(NodePorts &nodePorts, const std::map<QString, int> &options) const override;

    };
} // nitro
