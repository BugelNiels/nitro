#include "structuringelement.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_X "Width"
#define INPUT_Y "Height"
#define OUTPUT_IMAGE "Kernel"
#define MODE_DROPDOWN "Mode"

void nitro::StructElemOperator::execute(NodePorts &nodePorts) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    int option = nodePorts.getOption(MODE_DROPDOWN);
    int width = nodePorts.inputInteger(INPUT_X);
    int height = nodePorts.inputInteger(INPUT_Y);

    cv::MorphShapes shape;
    switch (option) {
        case 0:
            shape = cv::MORPH_RECT;
            break;
        case 1:
            shape = cv::MORPH_ELLIPSE;
            break;
        case 2:
            shape = cv::MORPH_CROSS;
            break;
        default:
            shape = cv::MORPH_RECT;
            break;
    }
    cv::Mat kernel = cv::getStructuringElement(shape, {width, height});
    cv::Mat result;
    kernel.convertTo(result, CV_32F);

    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::StructElemOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Structuring Element", "structuringElement", category);
        return builder.
                withOperator(std::make_unique<nitro::StructElemOperator>())->
                withIcon("kernel.png")->
                withNodeColor(NITRO_INPUT_COLOR)->
                withDropDown(MODE_DROPDOWN, {"Rectangle", "Ellipse", "Cross"})->
                withInputInteger(INPUT_X, 3, 1, 15)->
                withInputInteger(INPUT_Y, 3, 1, 15)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                build();
    };
}
