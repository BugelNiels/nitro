#include "imresize.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_WIDTH "Width"
#define INPUT_HEIGHT "Height"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

void nitro::ResizeOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {

    if (!nodePorts.inputsPresent({INPUT_IMAGE, INPUT_WIDTH, INPUT_HEIGHT})) {
        return;
    }
    int option = options.at(MODE_DROPDOWN);
    auto im1 = nodePorts.getInputImage(INPUT_IMAGE);
    int width = nodePorts.getInputInteger(INPUT_WIDTH);
    int height = nodePorts.getInputInteger(INPUT_HEIGHT);

    cv::InterpolationFlags mode;
    if (option == 0) {
        mode = cv::INTER_CUBIC;
    } else {
        mode = cv::INTER_NEAREST;
    }

    cv::Mat result;
    cv::resize(*im1, result, cv::Size(width, height), mode);

    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ResizeOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Resize", "resize", category);
        return builder.
                withOperator(std::make_unique<nitro::ResizeOperator>())->
                withIcon("resize.png")->
                withNodeColor({36, 98, 131})->
                withDropDown(MODE_DROPDOWN, {"Cubic", "Nearest-Neighbour"})->
                withInputImage(INPUT_IMAGE)->
                withInputInteger(INPUT_WIDTH, 256, 1, 4096)->
                withInputInteger(INPUT_HEIGHT, 256, 1, 4096)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
