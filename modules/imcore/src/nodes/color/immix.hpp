#pragma once

#include "nodes/nodeoperator.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    class MixOperator : public NodeOperator {
    public:
        static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

        void execute(NodePorts &nodePorts) override;

    private:
        cv::Mat fac_;
        cv::Mat in1_;
        cv::Mat in2_;

        void initUnifiedInputs(NodePorts &nodePorts);
    };
} // nitro
