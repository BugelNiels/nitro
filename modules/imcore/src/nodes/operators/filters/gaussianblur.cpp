#include "gaussianblur.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

void nitro::GaussianBlurOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {

    bool imPresent, sizePresent, sigmaPresent;
    auto inputImg = nodePorts.getInputImage("Image", imPresent);
    int kSize = nodePorts.getInputInteger("Size", sizePresent);
    double sigma = nodePorts.getInputValue("Sigma", sigmaPresent);
    if (!imPresent || !sizePresent || !sigmaPresent) {
        return;
    }
    cv::Mat result;
    kSize = kSize % 2 == 0 ? std::max(kSize - 1, 1) : kSize;
    cv::GaussianBlur(*inputImg, result, cv::Size(kSize, kSize), sigma, sigma);
    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::GaussianBlurOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Gaussian Blur", "gaussianFilter", category);
        return builder.
                withOperator(std::make_unique<nitro::GaussianBlurOperator>())->
                withIcon(":/icons/nodes/blur.png")->
                withNodeColor({71, 47, 189})->
                withInputImage("Image")->
                withInputInteger("Size", 5, 1, 64)->
                withInputValue("Sigma", 1, 0, 16)->
                withOutputImage("Image")->
                build();
    };
}
