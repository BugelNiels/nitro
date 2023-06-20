#include "imflip.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

void nitro::ImFlipOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    int option = options.at(MODE_DROPDOWN);
    auto im1 = nodePorts.inGet<ImageData>(INPUT_IMAGE).data();

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
    cv::flip(*im1, result, mode);
    nodePorts.output<ImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ImFlipOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Flip", "ImFlip", category);
        return builder.
                withOperator(std::make_unique<nitro::ImFlipOperator>())->
                withIcon("flip.png")->
                withNodeColor({60, 60, 131})->
                withDropDown(MODE_DROPDOWN, {"Horizontal", "Vertical", "Diagonal"})->
                withInputPort<ImageData>(INPUT_IMAGE)->
                withOutputPort<ImageData>(OUTPUT_IMAGE)->
                build();
    };
}
