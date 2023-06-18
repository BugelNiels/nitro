#include "immath.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_VALUE "Value"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

void nitro::MathOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {
    if (!nodePorts.inputsPresent({INPUT_VALUE, INPUT_IMAGE})) {
        return;
    }
    auto im1 = nodePorts.getInputImage(INPUT_IMAGE);
    double fac = nodePorts.getInputValue(INPUT_VALUE);
    int option = options.at(MODE_DROPDOWN);
    cv::Mat result;
    switch (option) {
        case 0: {
            result = *im1 + fac;
            break;
        }
        case 1: {
            result = *im1 - fac;
            break;
        }
        case 2:
            result = *im1 * fac;
            break;
        case 3:
            result = *im1 / fac;
            break;
        case 4:
            result = cv::min(*im1, fac);
            break;
        case 5:
            result = cv::max(*im1, fac);
            break;
        case 6:
            cv::pow(*im1, fac, result);
            break;
        case 7:
            cv::log(*im1, result);
            break;
        case 8:
            result = cv::abs(*im1);
            break;
        case 9:
            cv::sqrt(*im1, result);
            break;
        default:
            result = *im1 * fac;
            break;

    }

    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
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
                withInputImage(INPUT_IMAGE)->
                withInputValue(INPUT_VALUE, 0.5)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
