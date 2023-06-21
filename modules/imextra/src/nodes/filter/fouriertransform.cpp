#include "fouriertransform.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Image"
#define OPTION_INVERSE "Inverse"

void nitro::FFTOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    // Get the input data
    auto inputImg = GrayImageData::from(nodePorts.inGet(INPUT_IMAGE));
    int inverse = options.at(OPTION_INVERSE);

    cv::Mat result;
    cv::dft(*inputImg, result, inverse);

    // Store the result
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::FFTOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Fourier Transform", "fft", category);
        return builder.
                withOperator(std::make_unique<nitro::FFTOperator>())->
                withIcon("frequency.png")->
                withNodeColor(NITRO_FILTER_COLOR)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withCheckBox(OPTION_INVERSE, false)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                build();
    };
}
