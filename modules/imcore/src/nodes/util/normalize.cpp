#include "normalize.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_MIN "Min"
#define INPUT_MAX "Max"
#define OUTPUT_IMAGE "Image"

void nitro::NormalizeOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    auto inputImg = nodePorts.inGet<ImageData>(INPUT_IMAGE).data();
    double min = nodePorts.inputValue(INPUT_MIN);
    double max = nodePorts.inputValue(INPUT_MAX);
    cv::Mat result;
    cv::normalize(*inputImg, result, min, max, cv::NORM_MINMAX);

    nodePorts.output<ImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::NormalizeOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Normalize", "normalize", category);
        return builder.
                withOperator(std::make_unique<nitro::NormalizeOperator>())->
                withIcon("normalize.png")->
                withNodeColor({36, 98, 131})->
                withInputPort<ImageData>(INPUT_IMAGE)->
                withInputValue(INPUT_MIN, 0, 0, 1, BoundMode::UNCHECKED)->
                withInputValue(INPUT_MAX, 1, 0, 1, BoundMode::UNCHECKED)->
                withOutputPort<ImageData>(OUTPUT_IMAGE)->
                build();
    };
}
