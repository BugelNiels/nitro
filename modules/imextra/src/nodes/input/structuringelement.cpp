#include "structuringelement.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_WIDTH "Width"
#define INPUT_HEIGHT "Height"
#define OUTPUT_IMAGE "Kernel"
#define MODE_DROPDOWN "Mode"

void nitro::StructElemOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    int option = options.at(MODE_DROPDOWN);
    int width = nodePorts.inputInteger(INPUT_WIDTH);
    int height = nodePorts.inputInteger(INPUT_HEIGHT);

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

    nodePorts.output<ImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::StructElemOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Structuring Element", "structuringElement", category);
        return builder.
                withOperator(std::make_unique<nitro::StructElemOperator>())->
                withIcon("kernel.png")->
                withNodeColor(NITRO_INPUT_COLOR)->
                withDropDown(MODE_DROPDOWN, {"Rectangle", "Ellipse", "Cross"})->
                withInputInteger(INPUT_WIDTH, 3, 1, 15)->
                withInputInteger(INPUT_HEIGHT, 3, 1, 15)->
                withOutputPort<ImageData>(OUTPUT_IMAGE)->
                build();
    };
}
