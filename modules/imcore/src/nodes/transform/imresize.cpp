#include "imresize.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_WIDTH "Width"
#define INPUT_HEIGHT "Height"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"
#define INTERPOL_METHOD_LABEL "Interpolation Method"

void nitro::ResizeOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    int option = options.at(MODE_DROPDOWN);
    auto im1 = ColImageData::from(nodePorts.inGet(INPUT_IMAGE));
    int width = nodePorts.inputInteger(INPUT_WIDTH);
    int height = nodePorts.inputInteger(INPUT_HEIGHT);

    cv::InterpolationFlags mode;
    if (option == 0) {
        mode = cv::INTER_CUBIC;
    } else {
        mode = cv::INTER_NEAREST;
    }

    cv::Mat result;
    cv::resize(*im1, result, cv::Size(width, height), mode);

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
                withDropDown(MODE_DROPDOWN, {"Cubic", "Nearest-Neighbour"})->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_WIDTH, 256, 1, 2048, BoundMode::LOWER_ONLY)->
                withInputInteger(INPUT_HEIGHT, 256, 1, 2048, BoundMode::LOWER_ONLY)->
                withOutputPort<ColImageData>(OUTPUT_IMAGE)->
                build();
    };
}
