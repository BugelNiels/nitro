#include "normalize.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "include/colimagedata.hpp"

#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

inline const QString INPUT_IMAGE = "Image";
inline const QString INPUT_MIN = "Min";
inline const QString INPUT_MAX = "Max";
inline const QString OUTPUT_IMAGE = "Image";

void NormalizeOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto inputImg = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    double min = nodePorts.inputValue(INPUT_MIN);
    double max = nodePorts.inputValue(INPUT_MAX);
    cv::Mat result;
    cv::normalize(*inputImg, result, min, max, cv::NORM_MINMAX);

    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<NitroNode>()> NormalizeOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Normalize", "normalize", category);
        return builder.
                withOperator(std::make_unique<NormalizeOperator>())->
                withIcon("map_range.png")->
                withNodeColor(NITRO_CONVERTER_COLOR)->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                withInputValue(INPUT_MIN, 0, 0, 1, BoundMode::UNCHECKED)->
                withInputValue(INPUT_MAX, 1, 0, 1, BoundMode::UNCHECKED)->
                withOutputPort<ColImageData>(OUTPUT_IMAGE)->
                build();
    };
}

} // namespace nitro::ImCore
