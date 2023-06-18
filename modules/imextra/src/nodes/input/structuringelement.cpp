#include "structuringelement.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_WIDTH "Width"
#define INPUT_HEIGHT "Height"
#define OUTPUT_IMAGE "Kernel"
#define MODE_DROPDOWN "Mode"

void nitro::KernelOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {

    if (!nodePorts.inputsPresent({INPUT_WIDTH, INPUT_HEIGHT})) {
        return;
    }
    int option = options.at(MODE_DROPDOWN);
    int width = nodePorts.getInputInteger(INPUT_WIDTH);
    int height = nodePorts.getInputInteger(INPUT_HEIGHT);

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

    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::KernelOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Kernel", "kernel", category);
        return builder.
                withOperator(std::make_unique<nitro::KernelOperator>())->
                withIcon("kernel.png")->
                withNodeColor({131, 49, 74})->
                withDropDown(MODE_DROPDOWN, {"Rectangle", "Ellipse", "Cross"})->
                withInputInteger(INPUT_WIDTH, 3, 1, 15)->
                withInputInteger(INPUT_HEIGHT, 3, 1, 15)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
