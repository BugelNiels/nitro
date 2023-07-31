#pragma once

#include <nodes/nitronode.hpp>
#include <nodes/nodeoperator.hpp>

namespace nitro::ImProc {

/**
 * Node that performs Bilateral filtering on an input image.
 */
class BilateralFilterOperator : public NodeOperator {
public:
    /**
     * @brief Responsible for creating a function that builds this node.
     *
     * @param category Category name this node should be put in.
     * @return A function that creates this particular node.
     */
    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    /**
     * @brief Executes the Bilateral filtering algorithm of this node on the current input data.
     * @param nodePorts Port data containing the current input and output information.
     * @param options Options for passing additional parameters to the algorithm. Currently unused.
     */
    void execute(NodePorts &nodePorts) override;
};

} // namespace nitro::ImProc
