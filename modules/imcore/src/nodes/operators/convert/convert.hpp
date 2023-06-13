#pragma once

#include <opencv2/imgproc.hpp>
#include "nodes/nodeoperator.hpp"
#include "nodes/nitronode.hpp"

namespace nitro {

    class ConvertOperator : public NodeOperator {

    public:
        static std::function<std::unique_ptr<NitroNode>()> creator(const QString& category);

        explicit ConvertOperator(std::vector<cv::ColorConversionCodes> codes);

        void execute(NodePorts &nodePorts, const std::map<QString, int> &options) const override;

    private:
        std::vector<cv::ColorConversionCodes> codes_;

        static void getConversions(QStringList &colorNames, std::vector<cv::ColorConversionCodes> &codes);
    };

} // nitro
