#pragma once

#include "nodes/nodeoperator.hpp"
#include "gui/imviewer/imviewer.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    class ZLibOperator : public NodeOperator {
    public:

        explicit ZLibOperator(QLabel *valueLabel, QLabel *originalSizeLabel, QLabel *ratioLabel,
                              QLabel *timeLabel);

        static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

        void execute(NodePorts &nodePorts) override;
    private:
        QLabel* valueLabel_;
        QLabel* originalSizeLabel_;
        QLabel* ratioLabel_;
        QLabel* timeLabel_;
    };
}
