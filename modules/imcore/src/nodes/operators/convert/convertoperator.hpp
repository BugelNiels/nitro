#pragma once

#include <opencv2/imgproc.hpp>
#include "nodes/nodeoperator.hpp"

namespace nitro {

    class ConvertOperator : public NodeOperator {

    public:
        explicit ConvertOperator(std::vector<cv::ColorConversionCodes> codes);

        void execute(NodePorts &nodePorts, const std::map<QString, int> &options) const override;

    private:
        std::vector<cv::ColorConversionCodes> codes_;

    };

} // nitro
