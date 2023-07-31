#pragma once

#include <nodes/nitronode.hpp>
#include <nodes/nodeoperator.hpp>

namespace nitro::ImCore {

class ReductionOperator : public NodeOperator {
public:
    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    QLabel *typeLabel_;
};

} // namespace nitro::ImCore
