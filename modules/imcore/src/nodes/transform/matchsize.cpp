#include "mathsize.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include "util.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_IMAGE_TARGET "Target"
#define OPTION_ASPECT_RATIO "Keep Aspect Ratio"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"
#define INTERPOL_METHOD_LABEL "Interpolation Method"

void nitro::MatchSizeOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }

    int maintainAspectRatio = options.at(OPTION_ASPECT_RATIO);
    auto imIn = ColImageData::from(nodePorts.inGet(INPUT_IMAGE));
    auto imTarget = ColImageData::from(nodePorts.inGet(INPUT_IMAGE_TARGET));


    int option = options.at(MODE_DROPDOWN);
    cv::InterpolationFlags mode;
    if (option == 0) {
        mode = cv::INTER_LINEAR;
    } else if (option == 1) {
        mode = cv::INTER_CUBIC;
    } else {
        mode = cv::INTER_NEAREST;
    }
    cv::Mat result = nitro::resize(*imIn, imTarget->size(), mode, maintainAspectRatio);

    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);

}


std::function<std::unique_ptr<nitro::NitroNode>()> nitro::MatchSizeOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Math Size", "matchSize", category);
        return builder.
                withOperator(std::make_unique<nitro::MatchSizeOperator>())->
                withIcon("layers.png")->
                withNodeColor(NITRO_CONVERTER_COLOR)->
                withDisplayWidget(INTERPOL_METHOD_LABEL, new QLabel("Interpolation Method:"))->
                withDropDown(MODE_DROPDOWN, {"Linear", "Cubic", "Nearest-Neighbour"})->
                withCheckBox(OPTION_ASPECT_RATIO, true)->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                withInputPort<ColImageData>(INPUT_IMAGE_TARGET)->
                withOutputPort<ColImageData>(OUTPUT_IMAGE)->
                build();
    };
}
