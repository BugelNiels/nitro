#pragma once

#include <map>
#include <QString>
#include "QtNodes/NodeData"
#include "src/core/nodes/nodeports.hpp"

namespace nitro {

    class NodeOperator {
    public:
        virtual void execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {};
    };
}