#include "quantize.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#include <QDebug>

#define INPUT_IMAGE "Image"
#define INPUT_K "K"
#define OUTPUT_IMAGE "Image"

void nitro::QuantizeOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {
    if (!nodePorts.inputsPresent({INPUT_IMAGE, INPUT_K})) {
        return;
    }
    auto img = nodePorts.getInputImage(INPUT_IMAGE);
    int k = nodePorts.getInputInteger(INPUT_K);

    cv::Mat imIn;
    img->convertTo(imIn, CV_8U, 255);
    int max = getMaxValue(imIn);
    cv::Mat imDat = (imIn / max) * k + 0.5;
    cv::Mat quant = imDat * (max / double(k));
    cv::Mat result;
    quant.convertTo(result, CV_32F, 1 / 255.0);
    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::QuantizeOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Quantize", "quantization", category);
        return builder.
                withOperator(std::make_unique<nitro::QuantizeOperator>())->
                withIcon("quantize.png")->
                withNodeColor({43, 101, 43})->
                withInputImage(INPUT_IMAGE)->
                withInputInteger(INPUT_K, 8, 2, 255)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
