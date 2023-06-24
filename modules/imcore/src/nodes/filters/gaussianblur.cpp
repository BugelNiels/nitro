#include "gaussianblur.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_SIZE "Size"
#define INPUT_SIGMA "Sigma"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"
#define BORDER_DROPDOWN "Border"

void nitro::GaussianBlurOperator::execute(NodePorts &nodePorts) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    auto inputImg = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    int borderOption = nodePorts.getOption(BORDER_DROPDOWN);
    int kSize = nodePorts.inputInteger(INPUT_SIZE);
    double sigma = nodePorts.inputValue(INPUT_SIGMA);
    cv::Mat result;
    kSize = kSize % 2 == 0 ? std::max(kSize - 1, 1) : kSize;
    cv::GaussianBlur(*inputImg, result, cv::Size(kSize, kSize), sigma, sigma, borderOption);
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::GaussianBlurOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Gaussian Blur", "gaussianFilter", category);
        return builder.
                withOperator(std::make_unique<nitro::GaussianBlurOperator>())->
                withIcon("blur.png")->
                withNodeColor(NITRO_FILTER_COLOR)->
                withDropDown(BORDER_DROPDOWN, {"Constant", "Replicate", "Reflect"})->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_SIZE, 64, 1, 256, BoundMode::LOWER_ONLY)->
                withInputValue(INPUT_SIGMA, 32, 0, 128, BoundMode::LOWER_ONLY)->
                withOutputPort<ColImageData>(OUTPUT_IMAGE)->
                build();
    };
}
