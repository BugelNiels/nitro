#include "denoise.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

void nitro::DenoiseOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {

    bool imPresent;
    auto inputImg = nodePorts.getInputImage("Image", imPresent);
    if(!imPresent) {
        return;
    }
    cv::Mat result;
    cv::fastNlMeansDenoising(*inputImg, result);
    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::DenoiseOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Denoise", "denoise", category);
        return builder.
                withOperator(std::make_unique<nitro::DenoiseOperator>())->
                withIcon(":/icons/nodes/resample.png")->
                withNodeColor({71, 47, 189})->
                withInputImage("Image")->
                withOutputImage("Image")->
                build();
    };
}
