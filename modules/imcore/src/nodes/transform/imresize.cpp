#include "imresize.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_WIDTH "Width"
#define INPUT_HEIGHT "Height"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

void nitro::ResizeOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    int option = options.at(MODE_DROPDOWN);
    auto im1 = nodePorts.inGet<ImageData>(INPUT_IMAGE).data();
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

    nodePorts.output<ImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ResizeOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Resize", "resize", category);
        return builder.
                withOperator(std::make_unique<nitro::ResizeOperator>())->
                withIcon("resize.png")->
                withNodeColor(NITRO_TRANSFORM_COLOR)->
                withDropDown(MODE_DROPDOWN, {"Cubic", "Nearest-Neighbour"})->
                withInputPort<ImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_WIDTH, 256, 1, 2048, BoundMode::LOWER_ONLY)->
                withInputInteger(INPUT_HEIGHT, 256, 1, 2048, BoundMode::LOWER_ONLY)->
                withOutputPort<ImageData>(OUTPUT_IMAGE)->
                build();
    };
}
