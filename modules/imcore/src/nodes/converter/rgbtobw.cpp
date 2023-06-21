#include "rgbtobw.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Image"

void nitro::RgbToGrayscaleOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    auto inputImg = ColImageData::from(nodePorts.inGet(INPUT_IMAGE));
    cv::Mat result;
    if (inputImg->channels() == 1) {
        inputImg->copyTo(result);
    } else {
        cvtColor(*inputImg, result, cv::COLOR_RGB2GRAY);
    }
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::RgbToGrayscaleOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Grayscale Convert", "grayscaleConvert", category);
        return builder.
                withOperator(std::make_unique<nitro::RgbToGrayscaleOperator>())->
                withIcon("greyscale.png")->
                withNodeColor(NITRO_CONVERTER_COLOR)->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                build();
    };
}
