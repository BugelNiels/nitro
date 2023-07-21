#include "gaussianblur.hpp"
#include <nodes/nitronodebuilder.hpp>
#include "../../../../imcore/include/colimagedata.hpp"

#include <opencv2/imgproc.hpp>

namespace nitro::ImProc {

inline const QString INPUT_IMAGE = "Image";
inline const QString INPUT_SIZE = "Size";
inline const QString INPUT_SIGMA = "Sigma";
inline const QString OUTPUT_IMAGE = "Image";
inline const QString MODE_DROPDOWN = "Mode";
inline const QString BORDER_DROPDOWN = "Border";

void GaussianBlurOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
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

std::function<std::unique_ptr<NitroNode>()> GaussianBlurOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Gaussian Blur", "gaussianFilter", category);
        return builder.
                withOperator(std::make_unique<GaussianBlurOperator>())->
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

} // namespace nitro::ImProc
