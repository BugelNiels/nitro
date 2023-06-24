#pragma once

#include <map>
#include <QString>
#include "nodeports.hpp"

#include "categorycolors.h"

namespace nitro {

    class NodeOperator {
    public:
        virtual ~NodeOperator() = default;

        virtual void execute(NodePorts &nodePorts) = 0;
    };
}
