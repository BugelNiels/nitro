#pragma once

#include <nodes/nitronode.hpp>
#include <nodes/nodeoperator.hpp>

namespace nitro::ImCore {

class BooleanMathOperator : public NodeOperator {
public:
    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    cv::Mat in1_;
    cv::Mat in2_;

    void initUnifiedInputs(NodePorts &nodePorts);
};

} // namespace nitro::ImCore
