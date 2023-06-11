#include "convertoperator.hpp"
#include <opencv2/imgproc.hpp>
#include <utility>


nitro::ConvertOperator::ConvertOperator(std::vector<cv::ColorConversionCodes> codes) : codes_(std::move(codes)) {

}


void nitro::ConvertOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {


    cv::ColorConversionCodes codec = codes_[options.at("Mode")];

    bool imPresent;
    auto inputImg = nodePorts.getInputImage("Image", imPresent);
    if(!imPresent) {
        return;
    }

    cv::Mat result;
    if(inputImg->channels() == 1) {
        cv::cvtColor(*inputImg, result, cv::COLOR_GRAY2BGR);
    } else {
        inputImg->copyTo(result);
    }

    cvtColor(result, result, codec);

    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(result));


}
