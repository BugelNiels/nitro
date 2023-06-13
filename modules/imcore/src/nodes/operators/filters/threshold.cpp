#include "threshold.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_THRESH "Threshold"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

void nitro::ThresholdOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    // Retrieving of input parameters
    bool greater = options.at(MODE_DROPDOWN) == 1;
    auto type = greater ? cv::THRESH_BINARY : cv::THRESH_BINARY_INV;
    bool imPresent, tPresent;
    auto inputImg = nodePorts.getInputImage(INPUT_IMAGE);
    int threshold = nodePorts.getInputInteger(INPUT_THRESH);
    if (!imPresent || !tPresent) {
        return;
    }

    cv::Mat grayImage;
    inputImg->convertTo(grayImage, CV_8U, 255);

    // Apply thresholding to the grayscale image
    cv::Mat thresholded;
    cv::threshold(grayImage, thresholded, threshold, 255, type);

    // Actual Threshold operation
    cv::Mat result;
    thresholded.convertTo(result, CV_32F, 1.0 / 255.0);

    // Set node output
    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ThresholdOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Threshold", "threshold", category);
        return builder.
                withOperator(std::make_unique<nitro::ThresholdOperator>())->
                withIcon("threshold.png")->
                withNodeColor({71, 47, 189})->
                withDropDown(MODE_DROPDOWN, {"<=", ">="})->
                withInputImage(INPUT_IMAGE)->
                withInputInteger(INPUT_THRESH, 128, 0, 255)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
