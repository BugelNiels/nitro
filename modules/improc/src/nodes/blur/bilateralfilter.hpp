#pragma once

#include "nodes/nodeoperator.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    /**
     * Node that performs Bilateral filtering on an input image.
     */
    class BilateralFilterOperator : public NodeOperator {
    public:
        /**
         * Responsible for creating a function that builds this node.
         *
         * @param category Category name this node should be put in.
         * @return A function that creates this particular node.
         */
        static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

        /**
         * Executes the Bilateral filtering algorithm of this node on the current input data.
         * @param nodePorts Port data containing the current input and output information.
         * @param options Options for passing additional parameters to the algorithm. Currently unused.
         */
        void execute(NodePorts &nodePorts) override;

    };
} // nitro
