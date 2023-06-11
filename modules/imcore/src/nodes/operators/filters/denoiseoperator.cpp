#include "denoiseoperator.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

void nitro::DenoiseOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {

    bool imPresent;
    auto inputImg = nodePorts.getInputImage("Image", imPresent);
    if(!imPresent) {
        return;
    }
    cv::Mat result;
    cv::fastNlMeansDenoising(*inputImg, result);
    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(result));
}
