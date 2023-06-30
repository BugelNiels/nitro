#pragma once

#include "nodes/nodeoperator.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    class HistogramOperator : public NodeOperator {
    public:

        explicit HistogramOperator(QLabel *histLabel);

        static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

        void execute(NodePorts &nodePorts) override;

    private:
        QLabel* histLabel_;
        cv::Mat displayImage_;
    };
} // nitro
