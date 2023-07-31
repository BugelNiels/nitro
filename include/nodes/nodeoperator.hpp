#pragma once

#include "nodeports.hpp"
#include <QString>
#include <map>

#include "categorycolors.hpp"

namespace nitro {

/**
 * @brief A class that encapsulates the action executed when the node is evaluated. This is the primary component of the command pattern used by all the nodes.
 */
class NodeOperator {
public:
    /**
     * @brief Destructor.
     */
    virtual ~NodeOperator() = default;

    /**
     * @brief Executes the operation associated with the node. All input/output data and corresponding options are stored in the node ports. @see NodePorts.
     * @param nodePorts The node ports containing the input/output data and relevant options.
     */
    virtual void execute(NodePorts &nodePorts) = 0;
};

} // namespace nitro
