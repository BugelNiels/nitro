#include "gaussianblur.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_SIZE "Size"
#define INPUT_SIGMA "Sigma"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

void nitro::GaussianBlurOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    if (!nodePorts.inputsPresent({INPUT_IMAGE, INPUT_SIZE, INPUT_SIGMA})) {
        return;
    }
    auto inputImg = nodePorts.getInputImage(INPUT_IMAGE);
    int kSize = nodePorts.getInputInteger(INPUT_SIZE);
    double sigma = nodePorts.getInputValue(INPUT_SIGMA);
    cv::Mat result;
    kSize = kSize % 2 == 0 ? std::max(kSize - 1, 1) : kSize;
    cv::GaussianBlur(*inputImg, result, cv::Size(kSize, kSize), sigma, sigma);
    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::GaussianBlurOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Gaussian Blur", "gaussianFilter", category);
        return builder.
                withOperator(std::make_unique<nitro::GaussianBlurOperator>())->
                withIcon("blur.png")->
                withNodeColor({71, 47, 189})->
                withInputImage(INPUT_IMAGE)->
                withInputInteger(INPUT_SIZE, 5, 1, 64)->
                withInputValue(INPUT_SIGMA, 1, 0, 16)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
