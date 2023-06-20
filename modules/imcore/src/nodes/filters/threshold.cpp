#include "threshold.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_THRESH "Threshold"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

void nitro::ThresholdOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    bool greater = options.at(MODE_DROPDOWN) == 1;
    auto type = greater ? cv::THRESH_BINARY : cv::THRESH_BINARY_INV;
    auto inputImg = nodePorts.inGet<ImageData>(INPUT_IMAGE).data();
    int threshold = nodePorts.inputInteger(INPUT_THRESH);

    // Apply thresholding to the grayscale image
    cv::Mat result;
    cv::threshold(*inputImg, result, threshold / 255.0, 1, type);

    // Set node output
    nodePorts.output<ImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ThresholdOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Threshold", "threshold", category);
        return builder.
                withOperator(std::make_unique<nitro::ThresholdOperator>())->
                withIcon("threshold.png")->
                withNodeColor({71, 47, 189})->
                withDropDown(MODE_DROPDOWN, {"<=", ">="})->
                withInputPort<ImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_THRESH, 128, 0, 255)->
                withOutputPort<ImageData>(OUTPUT_IMAGE)->
                build();
    };
}
