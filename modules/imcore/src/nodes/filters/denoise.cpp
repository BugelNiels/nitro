#include "denoise.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Image"

void nitro::DenoiseOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    auto inputImg = nodePorts.inGet<ImageData>(INPUT_IMAGE).data();

    cv::Mat grayImage;
    inputImg->convertTo(grayImage, CV_8U, 255);

    cv::Mat result;
    cv::fastNlMeansDenoising(grayImage, result);

    result.convertTo(result, CV_32F, 1.0 / 255.0);
    nodePorts.output<ImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::DenoiseOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Denoise", "denoise", category);
        return builder.
                withOperator(std::make_unique<nitro::DenoiseOperator>())->
                withIcon("denoise.png")->
                withNodeColor({71, 47, 189})->
                withInputPort<ImageData>(INPUT_IMAGE)->
                withOutputPort<ImageData>(OUTPUT_IMAGE)->
                build();
    };
}
