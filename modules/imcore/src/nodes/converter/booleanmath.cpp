#include "booleanmath.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_VALUE_1 "Value 1"
#define INPUT_VALUE_2 "Value 2"
#define OUTPUT_VALUE "Value"
#define MODE_DROPDOWN "Mode"

double regularBoolMath(double a, double b, int option) {
    switch (option) {
        case 0: {
            return a < b;
        }
        case 1: {
            return a <= b;
        }
        case 2:
            return a > b;
        case 3:
            return a >= b;
        case 4:
            return a == b;
        case 5:
            return int(std::round(a)) && int(std::round(b));
        case 6:
            return int(std::round(a)) || int(std::round(b));
        case 7:
            return int(std::round(a)) ^ int(std::round(b));
        default:
            return a / 2.0 + b / 2.0;

    }
}

// Note this function doesn't support alpha images
static void match(const cv::Mat &src, cv::Mat &dest, const cv::Size &size, int numChannels) {
    src.copyTo(dest);
    if (dest.size() != size) {
        cv::resize(dest, dest, size);
    }
    if (dest.channels() != numChannels) {
        int conversionCode = 0;
        if (numChannels == 1 && src.channels() == 3) {
            conversionCode = cv::COLOR_RGB2GRAY;
        } else if (numChannels == 3 && src.channels() == 1) {
            conversionCode = cv::COLOR_GRAY2RGB;
        }
        cv::cvtColor(dest, dest, conversionCode);
    }
}

// ensures the images all have the same size and number of channels
void nitro::BooleanMathOperator::initUnifiedInputs(NodePorts &nodePorts) {
    auto in1 = *nodePorts.inGetAs<ColImageData>(INPUT_VALUE_1);
    auto in2 = *nodePorts.inGetAs<ColImageData>(INPUT_VALUE_2);

    int numChannels = std::max(in1.channels(), in2.channels());
    cv::Size size;
    if (in1.cols * in1.rows > in2.cols * in2.rows) {
        size = in1.size();
    } else {
        size = in2.size();
    }
    match(in1, in1_, size, numChannels);
    match(in2, in2_, size, numChannels);
}

void nitro::BooleanMathOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }

    // TODO: only update when changed
    int option = nodePorts.getOption(MODE_DROPDOWN);
    if (nodePorts.allInputOfType<DecimalData>()) {
        double in1 = nodePorts.inputValue(INPUT_VALUE_1);
        double in2 = nodePorts.inputValue(INPUT_VALUE_2);
        double result = regularBoolMath(in1, in2, option);
        nodePorts.output<DecimalData>(OUTPUT_VALUE, result);
        return;
    }

    initUnifiedInputs(nodePorts);

    cv::Mat result;
    switch (option) {
        case 0: {
            cv::compare(in1_, in2_, result, cv::CMP_LT);
            break;
        }
        case 1: {
            cv::compare(in1_, in2_, result, cv::CMP_LE);
            break;
        }
        case 2:
            cv::compare(in1_, in2_, result, cv::CMP_GT);
            break;
        case 3:
            cv::compare(in1_, in2_, result, cv::CMP_GE);
            break;
        case 4:
            cv::compare(in1_, in2_, result, cv::CMP_EQ);
            break;
        case 5:
            in1_.convertTo(in1_, CV_8U, 255);
            in2_.convertTo(in2_, CV_8U, 255);
            cv::bitwise_and(in1_, in2_, result);
            break;
        case 6:
            in1_.convertTo(in1_, CV_8U, 255);
            in2_.convertTo(in2_, CV_8U, 255);
            cv::bitwise_or(in1_, in2_, result);
            break;
        case 7:
            in1_.convertTo(in1_, CV_8U, 255);
            in2_.convertTo(in2_, CV_8U, 255);
            cv::bitwise_xor(in1_, in2_, result);
            break;
        default:
            cv::multiply(in1_, in2_, result);
            break;

    }
    nodePorts.output<ColImageData>(OUTPUT_VALUE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::BooleanMathOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Boolean Math", "booleanMath", category);
        return builder.
                withOperator(std::make_unique<nitro::BooleanMathOperator>())->
                withIcon("bool_math.png")->
                withNodeColor(NITRO_CONVERTER_COLOR)->
                withDropDown(MODE_DROPDOWN, {"<", "<=", ">", ">=", "==", "AND", "OR", "XOR"})->
                withInputValue(INPUT_VALUE_1, 0.5, 0, 1, BoundMode::UNCHECKED,
                               {ColImageData::id(), GrayImageData::id()})->
                withInputValue(INPUT_VALUE_2, 0.5, 0, 1, BoundMode::UNCHECKED,
                               {ColImageData::id(), GrayImageData::id()})->
                withOutputValue(OUTPUT_VALUE)->
                build();
    };
}
