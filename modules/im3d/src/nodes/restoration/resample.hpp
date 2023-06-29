#pragma once

#include "nodes/nodeoperator.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    class ResampleOperator : public NodeOperator {

    public:

        static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

        void execute(NodePorts &nodePorts) override;

    };

    cv::Mat resampleImage(const cv::Mat &img, int mode, int bits, bool brightnessCorrect);
} // nitro
