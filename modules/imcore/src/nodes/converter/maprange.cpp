#include "maprange.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Value"
#define INPUT_FROM_MIN "From Min"
#define INPUT_FROM_MAX "From Max"
#define INPUT_TO_MIN "To Min"
#define INPUT_TO_MAX "To Max"
#define OUTPUT_IMAGE "Result"
#define OPTION_CLAMP "Clamp"

void nitro::MapRangeOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    double fromMin = nodePorts.inputValue(INPUT_FROM_MIN);
    double fromMax = nodePorts.inputValue(INPUT_FROM_MAX);
    double toMin = nodePorts.inputValue(INPUT_TO_MIN);
    double toMax = nodePorts.inputValue(INPUT_TO_MAX);
    if (nodePorts.inputOfType<DecimalData>(INPUT_IMAGE)) {
        double value = nodePorts.inputValue(INPUT_IMAGE);
        value = (value - fromMin) / (fromMax - fromMin);
        value = toMin + value * (toMax - toMin);
        if (nodePorts.optionEnabled(OPTION_CLAMP)) {
            value = std::clamp(value, toMin, toMax);
        }
        nodePorts.output<DecimalData>(OUTPUT_IMAGE, value);
        return;
    }
    auto inputImg = nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    cv::Mat result;
    result = (*inputImg - fromMin) / (fromMax - fromMin);
    result = toMin + result * (toMax - toMin);

    if (nodePorts.optionEnabled(OPTION_CLAMP)) {
        cv::Scalar upper;
        cv::Scalar lower;
        if (result.channels() == 3) {
            upper = {toMax, toMax, toMax};
            lower = {toMin, toMin, toMin};
        } else {
            upper = {toMax};
            lower = {toMin};
        }
        result = cv::min(result, upper);
        result = cv::max(result, lower);
    }

    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::MapRangeOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Map Range", "mapRange", category);
        return builder.
                withOperator(std::make_unique<nitro::MapRangeOperator>())->
                withIcon("map_range.png")->
                withNodeColor(NITRO_CONVERTER_COLOR)->
                withInputValue(INPUT_IMAGE, 0.5, 0, 1, BoundMode::UNCHECKED,
                               {ColImageData::id(), GrayImageData::id()})->
                withInputValue(INPUT_FROM_MIN, 0, 0, 1, BoundMode::UNCHECKED)->
                withInputValue(INPUT_FROM_MAX, 1, 0, 1, BoundMode::UNCHECKED)->
                withInputValue(INPUT_TO_MIN, 0, 0, 1, BoundMode::UNCHECKED)->
                withInputValue(INPUT_TO_MAX, 1, 0, 1, BoundMode::UNCHECKED)->
                withOutputValue(OUTPUT_IMAGE)->
                withCheckBox(OPTION_CLAMP, false)->
                build();
    };
}
