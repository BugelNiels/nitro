#include "fouriertransform.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Image"
#define OPTION_INVERSE "Inverse"

void nitro::FFTOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    // Get the input data
    auto inputImg = nodePorts.inGet<ImageData>(INPUT_IMAGE).data();
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
    nodePorts.output<ImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::FFTOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Fourier Transform", "fft", category);
        return builder.
                withOperator(std::make_unique<nitro::FFTOperator>())->
                withIcon("frequency.png")->
                withNodeColor(NITRO_FILTER_COLOR)->
                withInputPort<ImageData>(INPUT_IMAGE)->
                withCheckBox(OPTION_INVERSE, false)->
                withOutputPort<ImageData>(OUTPUT_IMAGE)->
                build();
    };
}
