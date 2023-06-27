#pragma once

#include "nodes/nodeoperator.hpp"
#include "gui/imviewer/imviewer.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    class CompressionSizeOperator : public NodeOperator {
    public:

        explicit CompressionSizeOperator(QLabel *valueLabel, QLabel *originalSizeLabel,
                                         QLabel *ratioLabel);

        static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

        void execute(NodePorts &nodePorts) override;
    private:
        QLabel* valueLabel_;
        QLabel* originalSizeLabel_;
        QLabel* ratioLabel_;
    };
}
