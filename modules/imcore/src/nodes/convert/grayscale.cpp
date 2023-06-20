#include "grayscale.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Image"

// TODO: remove this node
// Change to color ramp?
void nitro::GrayscaleConvertOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    auto inputImg = nodePorts.inGet<ImageData>(INPUT_IMAGE).data();
    cv::Mat result;
    if (inputImg->channels() == 1) {
        cvtColor(*inputImg, result, cv::COLOR_GRAY2RGB);
    } else {
        cvtColor(*inputImg, result, cv::COLOR_RGB2GRAY);
    }
    nodePorts.output<ImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::GrayscaleConvertOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Grayscale Convert", "grayscaleConvert", category);
        return builder.
                withOperator(std::make_unique<nitro::GrayscaleConvertOperator>())->
                withIcon("greyscale.png")->
                withNodeColor({36, 98, 131})->
                withInputPort<ImageData>(INPUT_IMAGE)->
                withOutputPort<ImageData>(OUTPUT_IMAGE)->
                build();
    };
}
