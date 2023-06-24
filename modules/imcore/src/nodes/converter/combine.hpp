#pragma once

#include <opencv2/imgproc.hpp>
#include "nodes/nodeoperator.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    class CombineOperator : public NodeOperator {

    public:
        static std::function<std::unique_ptr<NitroNode>()> creator(const QString& category);

        void execute(NodePorts &nodePorts) override;

    };

} // nitro
