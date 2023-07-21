#include "fouriertransform.hpp"
#include <nodes/nitronodebuilder.hpp>
#include <grayimagedata.hpp>

#include <opencv2/imgproc.hpp>

namespace nitro::ImProc {

inline const QString INPUT_IMAGE = "Image";
inline const QString OUTPUT_IMAGE = "Image";
inline const QString OPTION_INVERSE = "Inverse";

void FFTOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    // Get the input data
    auto inputImg = nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    int inverse = nodePorts.getOption(OPTION_INVERSE);

    cv::Mat result;
    cv::dft(*inputImg, result, inverse);

    // Store the result
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<NitroNode>()> FFTOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Fourier Transform", "fft", category);
        return builder.
                withOperator(std::make_unique<FFTOperator>())->
                withIcon("frequency.png")->
                withNodeColor(NITRO_FILTER_COLOR)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withCheckBox(OPTION_INVERSE, false)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                build();
    };
}

} // namespace nitro::ImProc
