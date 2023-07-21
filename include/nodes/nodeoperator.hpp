#pragma once

#include <map>
#include <QString>
#include "nodeports.hpp"

#include "categorycolors.h"

namespace nitro {

/**
 * A class that encapsulates the action executed when the node is evaluated. This is the primary component of the command pattern used by all the nodes.
 */
class NodeOperator {
public:
    /**
     * Destructor.
     */
    virtual ~NodeOperator() = default;

    /**
     * Executes the operation associated with the node. All input/output data and corresponding options are stored in the node ports. @see NodePorts.
     * @param nodePorts The node ports containing the input/output data and relevant options.
     */
    virtual void execute(NodePorts &nodePorts) = 0;
};

} // namespace nitro
