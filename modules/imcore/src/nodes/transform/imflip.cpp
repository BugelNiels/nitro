#include "imflip.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

void nitro::ImFlipOperator::execute(NodePorts &nodePorts) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    int option = nodePorts.getOption(MODE_DROPDOWN);
    auto im1 = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);

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
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ImFlipOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Flip", "ImFlip", category);
        return builder.
                withOperator(std::make_unique<nitro::ImFlipOperator>())->
                withIcon("flip.png")->
                withNodeColor(NITRO_TRANSFORM_COLOR)->
                withDropDown(MODE_DROPDOWN, {"Horizontal", "Vertical", "Diagonal"})->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                withOutputPort<ColImageData>(OUTPUT_IMAGE)->
                build();
    };
}
