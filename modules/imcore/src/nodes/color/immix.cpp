#include "immix.hpp"
#include <util.hpp>
#include <nodes/nitronodebuilder.hpp>
#include "include/colimagedata.hpp"
#include <nodes/datatypes/decimaldata.hpp>
#include "include/grayimagedata.hpp"
#include <nodes/datatypes/integerdata.hpp>

#include <opencv2/imgproc.hpp>

namespace nitro::ImCore {

inline const QString INPUT_FAC = "Fac";
inline const QString INPUT_VALUE_1 = "Value 1";
inline const QString INPUT_VALUE_2 = "Value 2";
inline const QString OUTPUT_VALUE = "Value";
inline const QString OPTION_CLAMP = "Clamp";
inline const QString MODE_DROPDOWN = "Mode";

// Note this function doesn't support alpha images
static void match(const cv::Mat &src, cv::Mat &dest, const cv::Size &size, int numChannels) {
    src.copyTo(dest);
    if (dest.size() != size) {
        cv::resize(dest, dest, size);
    }
    if (dest.channels() != numChannels) {
        int conversionCode = 0;
        if (numChannels == 1 && dest.channels() == 3) {
            conversionCode = cv::COLOR_RGB2GRAY;
        } else if (numChannels == 3 && dest.channels() == 1) {
            conversionCode = cv::COLOR_GRAY2RGB;
        }
        cv::cvtColor(dest, dest, conversionCode);
    }
}

// ensures the images all have the same size and number of channels
void MixOperator::initUnifiedInputs(NodePorts &nodePorts) {
    auto fac = *nodePorts.inGetAs<GrayImageData>(INPUT_FAC);
    auto in1 = *nodePorts.inGetAs<ColImageData>(INPUT_VALUE_1);
    auto in2 = *nodePorts.inGetAs<ColImageData>(INPUT_VALUE_2);

    int numChannels = std::max({fac.channels(), in1.channels(), in2.channels()});
    cv::Size size;

    int area1 = fac.cols * fac.rows;
    int area2 = in1.cols * in1.rows;
    int area3 = in2.cols * in2.rows;

    int maxArea = std::max({area1, area2, area3});

    if (maxArea == area1) {
        size = fac.size();
    } else if (maxArea == area2) {
        size = in1.size();
    } else {
        size = in2.size();
    }

    fac.copyTo(fac_);
    if (fac_.size() != size) {
        cv::resize(fac_, fac_, size);
    }
    match(in1, in1_, size, numChannels);
    match(in2, in2_, size, numChannels);
}

void MixOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    int option = nodePorts.getOption(MODE_DROPDOWN);

    initUnifiedInputs(nodePorts);
    cv::Mat result;
    switch (option) {
        case 0: {
            cv::blendLinear(in1_, in2_, fac_, 1 - fac_, result);
            break;
        }
        default:
            cv::blendLinear(in1_, in2_, fac_, 1 - fac_, result);
            break;

    }

    if (nodePorts.optionEnabled(OPTION_CLAMP)) {
        cv::Scalar upper;
        cv::Scalar lower;
        if (result.channels() == 3) {
            upper = {1, 1, 1};
            lower = {0, 0, 0};
        } else {
            upper = {1};
            lower = {0};
        }
        result = cv::min(result, upper);
        result = cv::max(result, lower);
    }

    nodePorts.output<ColImageData>(OUTPUT_VALUE, result);
}

std::function<std::unique_ptr<NitroNode>()> MixOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Mix RGB", "mixRgb", category);
        return builder.
                withOperator(std::make_unique<MixOperator>())->
                withIcon("mix.png")->
                withNodeColor(NITRO_COLOR_COLOR)->
                withDropDown(MODE_DROPDOWN, {"Mix"})->
                withInputValue(INPUT_FAC, 1, 0, 1, BoundMode::UPPER_LOWER,
                               {ColImageData::id(), GrayImageData::id()})->
                withInputPort<ColImageData>(INPUT_VALUE_1, {IntegerData::id(), DecimalData::id()})->
                withInputPort<ColImageData>(INPUT_VALUE_2, {IntegerData::id(), DecimalData::id()})->
                withOutputValue(OUTPUT_VALUE)->
                withCheckBox(OPTION_CLAMP, false)->
                build();
    };
}

} // namespace nitro::ImCore
