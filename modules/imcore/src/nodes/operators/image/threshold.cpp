#include "threshold.hpp"
#include <opencv2/imgproc.hpp>


void nitro::ThresholdOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {

    bool greater = options.at("Mode") == 1;
    auto type = greater ? cv::THRESH_BINARY : cv::THRESH_BINARY_INV;

    bool imPresent, tPresent;
    auto inputImg = nodePorts.getInputImage("Image", imPresent);
    int threshold = nodePorts.getInputInteger("Threshold", tPresent);
    if(!imPresent || !tPresent) {
        return;
    }

    cv::Mat binaryMat(inputImg->size(), inputImg->type());
    cv::threshold(*inputImg, binaryMat, threshold, 255, type);

    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(binaryMat));


}
