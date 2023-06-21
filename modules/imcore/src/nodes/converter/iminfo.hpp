#pragma once

#include "nodes/nodeoperator.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    class ImInfoOperator : public NodeOperator {
    public:
        ImInfoOperator(QLabel *typeLabel);

        static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

        void execute(NodePorts &nodePorts, const std::map<QString, int> &options) override;

    private:
        QLabel *typeLabel_;
    };
} // nitro
