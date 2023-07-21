#pragma once

#include <nodes/nodeoperator.hpp>
#include <nodes/nitronode.hpp>

namespace nitro::ImProc {

class KMeansOperator : public NodeOperator {

public:

    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    void execute(NodePorts &nodePorts) override;

};

cv::Mat kMeansHist(const cv::Mat &image, int numColors, int maxIter);

} // namespace nitro::ImProc
