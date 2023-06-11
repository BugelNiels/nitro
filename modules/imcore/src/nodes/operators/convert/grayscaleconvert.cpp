#include "grayscaleconvert.hpp"
#include <opencv2/imgproc.hpp>
#include <utility>

void nitro::GrayscaleConvert::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {

    bool imPresent;
    auto inputImg = nodePorts.getInputImage("Image", imPresent);
    if(!imPresent) {
        return;
    }
    cv::Mat result;
    if(inputImg->channels() == 1) {
        cvtColor(*inputImg, result, cv::COLOR_GRAY2RGB);
    } else {
        cvtColor(*inputImg, result, cv::COLOR_RGB2GRAY);
    }
    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(result));
}
