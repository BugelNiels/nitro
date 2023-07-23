#pragma once

#include <nodes/nitronode.hpp>
#include <nodes/nodeoperator.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

class NormalizeOperator : public NodeOperator {

public:
    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;
};

} // namespace nitro::ImCore
