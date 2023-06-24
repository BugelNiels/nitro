#pragma once

#include "nodes/nodeoperator.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    class ColorMapOperator : public NodeOperator {
    public:
        explicit ColorMapOperator(QLabel *displayLabel);

        static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

        void execute(NodePorts &nodePorts) override;
    private:
        QLabel* displayLabel_;

        cv::Mat displayImage_;
    };
} // nitro
