#pragma once

#include "nodes/nodeoperator.hpp"
#include "nodes/nitronode.hpp"

namespace nitro::ImProc {


class ConnectedCompsOperator : public NodeOperator {
public:
    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

};

} // namespace nitro::ImProc
