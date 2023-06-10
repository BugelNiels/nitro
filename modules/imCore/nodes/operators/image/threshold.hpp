#pragma once

#include "modules/imCore/internal/cbdimage.hpp"
#include "include/nodes/nodeoperator.hpp"

namespace nitro {

    class ThresholdOperator : public NodeOperator {

        void execute(NodePorts &nodePorts, const std::map<QString, int> &options) const override;

    };

} // nitro
