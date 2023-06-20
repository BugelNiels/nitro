#pragma once

#include "nodes/nodeoperator.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    class ImageSourceOperator : public NodeOperator {
    public:

        static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

        void execute(NodePorts &nodePorts, const std::map<QString, int> &options) override;

    private:
        cv::Mat blankImg_ = {256, 256, CV_32FC1, cv::Scalar(0)};
    };
}
