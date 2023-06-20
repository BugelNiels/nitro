#pragma once

#include <map>
#include <QString>
#include "nodeports.hpp"

namespace nitro {

    class NodeOperator {
    public:
        virtual ~NodeOperator() = default;

        virtual void execute(NodePorts &nodePorts, const std::map<QString, int> &options) = 0;
    };
}
