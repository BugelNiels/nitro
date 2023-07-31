#include "imresize.hpp"
#include "include/colimagedata.hpp"
#include <nodes/nitronodebuilder.hpp>
#include <util.hpp>

#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString INPUT_X = "Width";
static inline const QString INPUT_Y = "Height";
static inline const QString OUTPUT_IMAGE = "Image";
static inline const QString MODE_DROPDOWN = "Mode";
static inline const QString ASPECT_RATIO_DROPDOWN = "Keep Aspect Ratio";
static inline const QString INTERPOL_METHOD_LABEL = "Interpolation Method";
static inline const QString AR_METHOD_LABEL = "Aspect Ratio Method";

void ResizeOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }

    AspectRatioMode arMode = static_cast<AspectRatioMode>(
            nodePorts.getOption(ASPECT_RATIO_DROPDOWN));
    int option = nodePorts.getOption(MODE_DROPDOWN);
    auto im1 = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    int width = nodePorts.inputInteger(INPUT_X);
    int height = nodePorts.inputInteger(INPUT_Y);

    cv::InterpolationFlags mode;
    if (option == 0) {
        mode = cv::INTER_LINEAR;
    } else if (option == 1) {
        mode = cv::INTER_CUBIC;
    } else {
        mode = cv::INTER_NEAREST;
    }
    cv::Mat result = resize(*im1, cv::Size(width, height), mode, arMode);
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<NitroNode>()> ResizeOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Resize", "resize", category);
        return builder.withOperator(std::make_unique<ResizeOperator>())
                ->withIcon("resize.png")
                ->withNodeColor(NITRO_TRANSFORM_COLOR)
                ->withDisplayWidget(INTERPOL_METHOD_LABEL, "Interpolation Method:")
                ->withDropDown(MODE_DROPDOWN, {"Linear", "Cubic", "Nearest-Neighbour"})
                ->withDisplayWidget(AR_METHOD_LABEL, "Aspect Ratio:")
                ->withDropDown(ASPECT_RATIO_DROPDOWN,
                               {"Ignore", "Keep Crop", "Keep Shrink", "Keep Grow"})
                ->withInputPort<ColImageData>(INPUT_IMAGE)
                ->withInputInteger(INPUT_X, 256, 2, 2048, BoundMode::LOWER_ONLY)
                ->withInputInteger(INPUT_Y, 256, 2, 2048, BoundMode::LOWER_ONLY)
                ->withOutputPort<ColImageData>(OUTPUT_IMAGE)
                ->build();
    };
}

} // namespace nitro::ImCore
