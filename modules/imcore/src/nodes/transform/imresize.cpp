#include "imresize.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_X "Width"
#define INPUT_Y "Height"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"
#define OPTION_ASPECT_RATIO "Keep Aspect Ratio"
#define INTERPOL_METHOD_LABEL "Interpolation Method"

void nitro::ResizeOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }

    int maintainAspectRatio = nodePorts.getOption(OPTION_ASPECT_RATIO);
    int option = nodePorts.getOption(MODE_DROPDOWN);
    auto im1 = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);;
    int width = nodePorts.inputInteger(INPUT_X);
    int height = nodePorts.inputInteger(INPUT_Y);

    cv::InterpolationFlags mode;
    if (option == 0) {
        mode = cv::INTER_LINEAR;
    } else if (option == 1) {
        mode = cv::INTER_CUBIC;
    } else {
        mode = cv::INTER_NEAREST;
    }
    cv::Mat result = nitro::resize(*im1, cv::Size(width, height), mode, maintainAspectRatio);
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ResizeOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Resize", "resize", category);
        return builder.
                withOperator(std::make_unique<nitro::ResizeOperator>())->
                withIcon("resize.png")->
                withNodeColor(NITRO_TRANSFORM_COLOR)->
                withDisplayWidget(INTERPOL_METHOD_LABEL, new QLabel("Interpolation Method:"))->
                withDropDown(MODE_DROPDOWN, {"Linear", "Cubic", "Nearest-Neighbour"})->
                withCheckBox(OPTION_ASPECT_RATIO, true)->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_X, 256, 1, 2048, BoundMode::LOWER_ONLY)->
                withInputInteger(INPUT_Y, 256, 1, 2048, BoundMode::LOWER_ONLY)->
                withOutputPort<ColImageData>(OUTPUT_IMAGE)->
                build();
    };
}
