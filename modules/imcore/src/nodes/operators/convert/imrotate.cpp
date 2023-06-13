#include "imrotate.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

void nitro::ImRotateOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {

    if (!nodePorts.inputsPresent({INPUT_IMAGE})) {
        return;
    }
    int option = options.at(MODE_DROPDOWN);
    auto im1 = nodePorts.getInputImage(INPUT_IMAGE);

    cv::RotateFlags mode;
    switch (option) {
        case 0:
            mode = cv::ROTATE_90_CLOCKWISE;
            break;
        case 1 :
            mode = cv::ROTATE_90_COUNTERCLOCKWISE;
            break;
        default:
            mode = cv::ROTATE_180;
            break;
    }

    cv::Mat result;
    cv::rotate(*im1, result, mode);

    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ImRotateOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Rotate", "rotate", category);
        return builder.
                withOperator(std::make_unique<nitro::ImRotateOperator>())->
                withIcon("rotate.png")->
                withNodeColor({36, 98, 131})->
                withDropDown(MODE_DROPDOWN, {"90 (CW)", "90 (CCw)", "180"})->
                withInputImage(INPUT_IMAGE)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
