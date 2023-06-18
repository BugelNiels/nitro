#include "fouriertransform.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Image"
#define OPTION_INVERSE "Inverse"

void nitro::FFTOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    // Verifying that all the required inputs are there
    if (!nodePorts.inputsPresent({INPUT_IMAGE})) {
        return;
    }
    // Get the input data
    auto inputImg = nodePorts.getInputImage(INPUT_IMAGE);
    int inverse = options.at(OPTION_INVERSE);

    cv::Mat imIn;
    if (inputImg->channels() > 1) {
        cvtColor(*inputImg, imIn, cv::COLOR_RGB2GRAY);
    } else {
        imIn = *inputImg;
    }

    cv::Mat result;
    cv::dft(imIn, result, inverse);

    // Store the result
    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::FFTOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("FFT", "fft", category);
        return builder.
                withOperator(std::make_unique<nitro::FFTOperator>())->
                withIcon("frequency.png")->
                withNodeColor({47, 105, 204})->
                withInputImage(INPUT_IMAGE)->
                withCheckBox(OPTION_INVERSE, false)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
