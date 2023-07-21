#pragma once

#include "nodes/nodeoperator.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    class ConnectedCompsOperator : public NodeOperator {
    public:
        static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);
        void execute(NodePorts &nodePorts) override;

    };
} // nitro
