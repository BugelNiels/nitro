#pragma once

#include <nodes/nitronode.hpp>
#include <nodes/nodeoperator.hpp>

namespace nitro::Thesis {

class ResampleOperator : public NodeOperator {

public:
    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;
};

cv::Mat resampleImage(const cv::Mat &img, bool brightnessCorrect, double kSize = 0.125);

} // namespace nitro::Thesis
