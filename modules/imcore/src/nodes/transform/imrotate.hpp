#pragma once

#include <nodes/nitronode.hpp>
#include <nodes/nodeoperator.hpp>

namespace nitro::ImCore {

class ImRotateOperator : public NodeOperator {
public:
    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;
};

} // namespace nitro::ImCore
