#pragma once

#include "nodes/nodeoperator.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    class CompressOperator : public NodeOperator {
    public:
        explicit CompressOperator(QLabel *timeLabel);

        static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);
        void execute(NodePorts &nodePorts) override;

    private:
        QLabel* timeLabel_;
    };
} // nitro
