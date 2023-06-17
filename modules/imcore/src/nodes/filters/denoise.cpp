#include "denoise.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Image"

void nitro::DenoiseOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    if (!nodePorts.inputsPresent({INPUT_IMAGE})) {
        return;
    }
    auto inputImg = nodePorts.getInputImage(INPUT_IMAGE);

    cv::Mat grayImage;
    inputImg->convertTo(grayImage, CV_8U, 255);

    // Apply thresholding to the grayscale image
    cv::Mat denoised;
    cv::fastNlMeansDenoising(grayImage, denoised);

    // Actual Threshold operation
    cv::Mat result;
    denoised.convertTo(result, CV_32F, 1.0 / 255.0);
    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::DenoiseOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Denoise", "denoise", category);
        return builder.
                withOperator(std::make_unique<nitro::DenoiseOperator>())->
                withIcon("resample.png")->
                withNodeColor({71, 47, 189})->
                withInputImage(INPUT_IMAGE)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
