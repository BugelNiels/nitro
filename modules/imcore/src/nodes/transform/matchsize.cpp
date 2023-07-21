#include "matchsize.hpp"
#include <nodes/nitronodebuilder.hpp>
#include "include/colimagedata.hpp"
#include <util.hpp>

#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

inline const QString INPUT_IMAGE = "Image";
inline const QString INPUT_IMAGE_TARGET = "Target";
inline const QString ASPECT_RATIO_DROPDOWN = "Keep Aspect Ratio";
inline const QString OUTPUT_IMAGE = "Image";
inline const QString MODE_DROPDOWN = "Mode";
inline const QString INTERPOL_METHOD_LABEL = "Interpolation Method";
inline const QString AR_METHOD_LABEL = "Aspect Ratio Method";

void MatchSizeOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }

    AspectRatioMode arMode = static_cast<AspectRatioMode>(nodePorts.getOption(ASPECT_RATIO_DROPDOWN));
    auto imIn = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    auto imTarget = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE_TARGET);


    int option = nodePorts.getOption(MODE_DROPDOWN);
    cv::InterpolationFlags mode;
    if (option == 0) {
        mode = cv::INTER_LINEAR;
    } else if (option == 1) {
        mode = cv::INTER_CUBIC;
    } else {
        mode = cv::INTER_NEAREST;
    }
    cv::Mat result = nitro::resize(*imIn, imTarget->size(), mode, arMode);

    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);

}


std::function<std::unique_ptr<NitroNode>()> MatchSizeOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Match Size", "matchSize", category);
        return builder.
                withOperator(std::make_unique<MatchSizeOperator>())->
                withIcon("match_size.png")->
                withNodeColor(NITRO_TRANSFORM_COLOR)->
                withDisplayWidget(INTERPOL_METHOD_LABEL, "Interpolation Method:")->
                withDropDown(MODE_DROPDOWN, {"Linear", "Cubic", "Nearest-Neighbour"})->
                withDisplayWidget(AR_METHOD_LABEL, "Aspect Ratio:")->
                withDropDown(ASPECT_RATIO_DROPDOWN, {"Ignore", "Keep Crop", "Keep Shrink", "Keep Grow"})->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                withInputPort<ColImageData>(INPUT_IMAGE_TARGET)->
                withOutputPort<ColImageData>(OUTPUT_IMAGE)->
                build();
    };
}

} // namespace nitro::ImCore
