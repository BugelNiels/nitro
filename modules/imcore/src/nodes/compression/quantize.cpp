#include "quantize.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include <opencv2/imgproc.hpp>

#include <QDebug>

#define INPUT_IMAGE "Image"
#define INPUT_K "K"
#define OUTPUT_IMAGE "Image"

void nitro::QuantizeOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    auto img = nodePorts.inGet<ImageData>(INPUT_IMAGE).data();
    int k = nodePorts.inputInteger(INPUT_K);

    cv::Mat imIn;
    img->convertTo(imIn, CV_8U, 255);
    int max = getMaxValue(imIn);
    cv::Mat imDat = (imIn / max) * k + 0.5;
    cv::Mat quant = imDat * (max / double(k));
    cv::Mat result;
    quant.convertTo(result, CV_32F, 1 / 255.0);
    nodePorts.output<ImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::QuantizeOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Quantize", "quantization", category);
        return builder.
                withOperator(std::make_unique<nitro::QuantizeOperator>())->
                withIcon("quantize.png")->
                withNodeColor(NITRO_COMPRESSION_COLOR)->
                withInputPort<ImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_K, 8, 2, 255)->
                withOutputPort<ImageData>(OUTPUT_IMAGE)->
                build();
    };
}
