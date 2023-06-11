#include "bilateralfilteroperator.hpp"
#include <opencv2/imgproc.hpp>

void nitro::BilateralFilterOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {

    bool imPresent, sigmaColPresent, sigmaSpacePresent, dPresent;
    auto inputImg = nodePorts.getInputImage("Image", imPresent);
    double sigmaCol = nodePorts.getInputValue("Sigma (c)", sigmaColPresent);
    double sigmaSpace = nodePorts.getInputValue("Sigma (s)", sigmaSpacePresent);
    int d = nodePorts.getInputInteger("d", dPresent);
    if (!imPresent || !sigmaColPresent || !sigmaSpacePresent || !dPresent) {
        return;
    }
    cv::Mat result;
    cv::bilateralFilter(*inputImg, result, d, sigmaCol, sigmaSpace);
    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(result));
}
