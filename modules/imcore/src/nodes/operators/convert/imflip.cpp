#include "imflip.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

void nitro::ImFlipOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {

    if (!nodePorts.inputsPresent({INPUT_IMAGE})) {
        return;
    }
    int option = options.at(MODE_DROPDOWN);
    auto im1 = nodePorts.getInputImage(INPUT_IMAGE);

    int mode;
    switch (option) {
        case 0:
            mode = 1;
            break;
        case 1 :
            mode = 0;
            break;
        default:
            mode = -1;
            break;
    }

    cv::Mat result;
    cv::Mat resizedImage;
    cv::flip(*im1, result, mode);
    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ImFlipOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Flip", "ImFlip", category);
        return builder.
                withOperator(std::make_unique<nitro::ImFlipOperator>())->
                withIcon("flip.png")->
                withNodeColor({36, 98, 131})->
                withDropDown(MODE_DROPDOWN, {"Horizontal", "Vertical", "Diagonal"})->
                withInputImage(INPUT_IMAGE)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
