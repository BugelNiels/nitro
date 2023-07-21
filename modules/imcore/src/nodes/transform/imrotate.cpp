#include "imrotate.hpp"
#include <util.hpp>
#include <nodes/nitronodebuilder.hpp>
#include "include/colimagedata.hpp"

#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

inline const QString INPUT_IMAGE = "Image";
inline const QString OUTPUT_IMAGE = "Image";
inline const QString MODE_DROPDOWN = "Mode";

void ImRotateOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    int option = nodePorts.getOption(MODE_DROPDOWN);
    auto im1 = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);

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

    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<NitroNode>()> ImRotateOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Rotate", "rotate", category);
        return builder.
                withOperator(std::make_unique<ImRotateOperator>())->
                withIcon("rotate.png")->
                withNodeColor(NITRO_TRANSFORM_COLOR)->
                withDropDown(MODE_DROPDOWN, {"90 (CW)", "90 (CCw)", "180"})->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                withOutputPort<ColImageData>(OUTPUT_IMAGE)->
                build();
    };
}

} // namespace nitro::ImCore
