#pragma once

#include "nodes/nodeoperator.hpp"
#include "nodes/nitronode.hpp"

namespace nitro::ImCore {

// Note that this is a temporary node until NITRO supports Histogram and detailed color views in the image viewer.
class HistogramOperator : public NodeOperator {
public:

    explicit HistogramOperator(QLabel *histLabel);

    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

private:
    QLabel *histLabel_;
    cv::Mat displayImage_;
};

} // namespace nitro::ImCore
