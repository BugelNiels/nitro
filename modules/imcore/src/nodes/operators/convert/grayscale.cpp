#include "grayscale.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Image"

void nitro::GrayscaleConvert::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    auto inputImg = nodePorts.getInputImage(INPUT_IMAGE);
    if (!nodePorts.inputsPresent({INPUT_IMAGE})) {
        return;
    }
    cv::Mat result;
    if (inputImg->channels() == 1) {
        cvtColor(*inputImg, result, cv::COLOR_GRAY2RGB);
    } else {
        cvtColor(*inputImg, result, cv::COLOR_RGB2GRAY);
    }
    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::GrayscaleConvert::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Grayscale Convert", "grayscaleConvert", category);
        return builder.
                withOperator(std::make_unique<nitro::GrayscaleConvert>())->
                withIcon("greyscale.png")->
                withNodeColor({36, 98, 131})->
                withInputImage(INPUT_IMAGE)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
