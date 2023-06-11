#include "quantize.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#include <QDebug>

void nitro::QuantizeOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {
    bool kPresent, imPresent;
    int k = nodePorts.getInputInteger("K", kPresent);
    auto img = nodePorts.getInputImage("Image", imPresent);

    if (!kPresent || !imPresent) {
        return;
    }

    int max = getMaxValue(*img);
    cv::Mat imDat = (*img / max) * (k) + 0.5;
    cv::Mat result =  imDat * (max / double(k));

    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::QuantizeOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Quantize", "quantization", category);
        return builder.
                withOperator(std::make_unique<nitro::QuantizeOperator>())->
                withIcon(":/icons/nodes/quantize.png")->
                withNodeColor({43, 101, 43})->
                withInputImage("Image")->
                withInputInteger("K", 8, 1, 255)->
                withOutputImage("Image")->
                build();
    };
}
