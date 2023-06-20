#include "immath.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_FAC "Fac"
#define INPUT_VALUE_1 "Value 1"
#define INPUT_VALUE_2 "Value 2"
#define OUTPUT_VALUE "Value"
#define MODE_DROPDOWN "Mode"

double regularMath(double fac, double a, double b, int option) {
    double result;
    switch (option) {
        case 0: {
            result = a + b;
            break;
        }
        case 1: {
            result = a - b;
            break;
        }
        case 2:
            result = a * b;
            break;
        case 3:
            result = a / b;
            break;
        case 4:
            result = std::min(a, b);
            break;
        case 5:
            result = std::max(a, b);
            break;
        case 6:
            result = std::pow(a, b);
            break;
        case 7:
            result = std::log(a) / std::log(b);
            break;
        case 8:
            result = std::abs(a);
            break;
        case 9:
            result = std::sqrt(a);
            break;
        default:
            result = a * b;
            break;

    }
    return (1 - fac) * a + fac * result;
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
void nitro::MathOperator::initUnifiedInputs(NodePorts &nodePorts) {
    auto fac = *ImageData::from(nodePorts.inGet(INPUT_FAC));
    auto in1 = *ImageData::from(nodePorts.inGet(INPUT_VALUE_1));
    auto in2 = *ImageData::from(nodePorts.inGet(INPUT_VALUE_2));

    int numChannels = std::max({fac.channels(), in1.channels(), in2.channels()});
    cv::Size size;

    int area1 = fac.cols * fac.rows;
    int area2 = in1.cols * in1.rows;
    int area3 = in2.cols * in2.rows;

    // Find the maximum area among the three images
    int maxArea = std::max(area1, std::max(area2, area3));

    // Return the image with the maximum area
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
    if (fac_.channels() != 1) {
        cv::cvtColor(fac_, fac_, cv::COLOR_RGB2GRAY);
    }
    match(in1, in1_, size, numChannels);
    match(in2, in2_, size, numChannels);
}

void nitro::MathOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }

    // TODO: only update when changed
    int option = options.at(MODE_DROPDOWN);
    if (nodePorts.allInputOfType<DecimalData>()) {
        double facDouble = nodePorts.inputValue(INPUT_FAC);
        double in1 = nodePorts.inputValue(INPUT_VALUE_1);
        double in2 = nodePorts.inputValue(INPUT_VALUE_2);
        double result = regularMath(facDouble, in1, in2, option);
        nodePorts.output<DecimalData>(OUTPUT_VALUE, result);
        return;
    }

    initUnifiedInputs(nodePorts);

    cv::Mat result;
    switch (option) {
        case 0: {
            cv::add(in1_, in2_, result);
            break;
        }
        case 1: {
            cv::subtract(in1_, in2_, result);
            break;
        }
        case 2:
            cv::multiply(in1_, in2_, result);
            break;
        case 3:
            cv::divide(in1_, in2_, result);
            break;
        case 4:
            result = cv::min(in1_, in2_);
            break;
        case 5:
            result = cv::max(in1_, in2_);
            break;
        case 6:
            // TODO: implement manually
//            cv::pow(*im1, *im2, result);
            break;
        case 7: {
            cv::Mat temp;
            cv::log(in1_, result);
            cv::log(in1_, temp);
            cv::divide(result, temp, result);
            break;
        }
        case 8:
            // TODO: disable port
            result = cv::abs(in1_);
            break;
        case 9:
            // TODO: disable port
            cv::sqrt(in1_, result);
            break;
        default:
            cv::multiply(in1_, in2_, result);
            break;

    }
    cv::blendLinear(result, in1_, 1 - fac_, fac_, result);
    nodePorts.output<ImageData>(OUTPUT_VALUE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::MathOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Math", "math", category);
        return builder.
                withOperator(std::make_unique<nitro::MathOperator>())->
                withIcon("math.png")->
                withNodeColor({36, 98, 131})->
                withDropDown(MODE_DROPDOWN, {"Add", "Subtract", "Multiply", "Divide", "Min", "Max", "Pow", "Log", "Abs",
                                             "Square Root"})->
                withInputValue(INPUT_FAC, 0.5, 0, 1)->
                withInputValue(INPUT_VALUE_1, 0.5, 0, 1, BoundMode::UNCHECKED)->
                withInputValue(INPUT_VALUE_2, 0.5, 0, 1, BoundMode::UNCHECKED)->
                withOutputValue(OUTPUT_VALUE)->
                build();
    };
}
