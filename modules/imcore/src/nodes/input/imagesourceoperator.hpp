#pragma once

#include <nodes/nitronode.hpp>
#include <nodes/nodeoperator.hpp>

namespace nitro::ImCore {

class ImageSourceOperator : public NodeOperator {
public:
    ImageSourceOperator(QLabel *displayImgLabel);

    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    cv::Mat blankImg_ = {256, 256, CV_32FC1, cv::Scalar(0)};
    QLabel *displayImgLabel_;
    cv::Mat displayBuf_;
};

} // namespace nitro::ImCore
