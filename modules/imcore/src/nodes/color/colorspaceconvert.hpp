#pragma once

#include <nodes/nitronode.hpp>
#include <nodes/nodeoperator.hpp>
#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

class ConvertOperator : public NodeOperator {

public:
    static std::function<std::unique_ptr<NitroNode>()> creator(const QString &category);

    explicit ConvertOperator(std::vector<cv::ColorConversionCodes> codes);

    void execute(NodePorts &nodePorts) override;

private:
    std::vector<cv::ColorConversionCodes> codes_;

    static void getConversions(QStringList &colorNames,
                               std::vector<cv::ColorConversionCodes> &codes);
};

} // namespace nitro::ImCore
