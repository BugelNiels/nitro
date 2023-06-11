#include "threshold.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

void nitro::ThresholdOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    // Retrieving of input parameters
    bool greater = options.at("Mode") == 1;
    auto type = greater ? cv::THRESH_BINARY : cv::THRESH_BINARY_INV;
    bool imPresent, tPresent;
    auto inputImg = nodePorts.getInputImage("Image", imPresent);
    int threshold = nodePorts.getInputInteger("Threshold", tPresent);
    if(!imPresent || !tPresent) {
        return;
    }

    // Actual Threshold operation
    cv::Mat result;
    cv::threshold(*inputImg, result, threshold, 255, type);

    // Set node output
    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ThresholdOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Threshold", "threshold", category);
        return builder.
                withOperator(std::make_unique<nitro::ThresholdOperator>())->
                withIcon(":/icons/nodes/threshold.png")->
                withNodeColor({43, 101, 43})->
                withDropDown("Mode", {"<=", ">="})->
                withInputImage("Image")->
                withInputInteger("Threshold", 128, 0, 255)->
                withOutputImage("Image")->
                build();
    };
}
