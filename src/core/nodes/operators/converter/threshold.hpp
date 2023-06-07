#pragma once

#include "src/core/cbdimage.hpp"
#include "src/core/nodes/operators/nodeoperator.hpp"

namespace nitro {

    CbdImage threshold(const CbdImage &inputImg, int threshold, bool greater = false);

    class ThresholdAlgorithm : public NodeOperator {

        void execute(NodePorts &nodePorts, const std::map<QString, int> &options) const override;

    };

} // nitro::operations
