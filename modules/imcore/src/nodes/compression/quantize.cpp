#include "quantize.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include <opencv2/imgproc.hpp>

#include <QDebug>

#define INPUT_IMAGE "Image"
#define INPUT_K "K"
#define OUTPUT_IMAGE "Image"

void nitro::QuantizeOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto img = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);;
    int k = nodePorts.inputInteger(INPUT_K) - 1;

    cv::Mat result = *img * k;
    cv::Mat imIn;
    result.convertTo(imIn, CV_8U);
    imIn.convertTo(result, CV_32F, 1.0 / k);
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::QuantizeOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Quantize", "quantization", category);
        return builder.
                withOperator(std::make_unique<nitro::QuantizeOperator>())->
                withIcon("quantize.png")->
                withNodeColor(NITRO_COMPRESSION_COLOR)->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_K, 8, 2, 255)->
                withOutputPort<ColImageData>(OUTPUT_IMAGE)->
                build();
    };
}
