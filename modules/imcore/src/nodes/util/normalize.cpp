#include "normalize.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_MIN "Min"
#define INPUT_MAX "Max"
#define OUTPUT_IMAGE "Image"

void nitro::NormalizeOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    auto inputImg = nodePorts.getInputImage(INPUT_IMAGE);
    if (!nodePorts.inputsPresent({INPUT_IMAGE})) {
        return;
    }
    double min = nodePorts.getInputValue(INPUT_MIN);
    double max = nodePorts.getInputValue(INPUT_MAX);
    cv::Mat result;
    cv::normalize(*inputImg, result, min, max, cv::NORM_MINMAX);

    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::NormalizeOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Normalize", "normalize", category);
        return builder.
                withOperator(std::make_unique<nitro::NormalizeOperator>())->
                withIcon("normalize.png")->
                withNodeColor({36, 98, 131})->
                withInputImage(INPUT_IMAGE)->
                withInputValue(INPUT_MIN, 0)->
                withInputValue(INPUT_MAX, 1)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
