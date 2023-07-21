#include "uniformconvert.hpp"
#include <nodes/nitronodebuilder.hpp>
#include "include/grayimagedata.hpp"

#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

inline const QString INPUT_IMAGE = "Image";
inline const QString OUTPUT_IMAGE = "Image";
inline const QString OPTION_INVERSE = "Inverse";
inline const QString OPTION_ROUND = "Round";

void UniformConvertOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto inputImg = *nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    cv::Mat result;

    cv::cvtColor(inputImg, result, cv::COLOR_GRAY2RGB);

    int mode = nodePorts.getOption(OPTION_INVERSE);
    if (mode == 0) {
        cvtColor(result, result, cv::COLOR_RGB2Lab);
        if (nodePorts.optionEnabled(OPTION_ROUND)) {
            cv::Mat temp;
            result.convertTo(temp, CV_8U);
            temp.convertTo(result, CV_32F);
        }
        std::vector<cv::Mat> channels;
        cv::split(result, channels);
        result = channels[0] / 100.0;
    } else {
        std::vector<cv::Mat> channels;
        cv::split(result, channels);
        channels[0] = channels[0] * 100.0;
        cv::merge(channels, result);
        cvtColor(result, result, cv::COLOR_Lab2RGB);
        cv::split(result, channels);
        result = channels[0];
    }

    // Store the result
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<NitroNode>()> UniformConvertOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Convert Luminance", "perceptUniform", category);
        return builder.
                withOperator(std::make_unique<UniformConvertOperator>())->
                withIcon("gradient.png")->
                withNodeColor(NITRO_COLOR_COLOR)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withDropDown(OPTION_INVERSE, {"Gray -> L", "L -> Gray"})->
                withCheckBox(OPTION_ROUND, false)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                build();
    };
}

} // namespace nitro::ImCore
