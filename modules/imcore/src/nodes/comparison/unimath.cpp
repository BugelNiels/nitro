#include "unimath.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_FAC "Fac"
#define INPUT_1 "Value 1"
#define INPUT_2 "Value"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

cv::Mat math(std::shared_ptr<nitro::DecimalData> fac, std::shared_ptr<nitro::DecimalData> in1, std::shared_ptr<nitro::DecimalData> in2) {

}

void nitro::UniMathOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {
    if (!nodePorts.inputsPresent({INPUT_1, INPUT_2, INPUT_FAC})) {
        return;
    }

    auto in1 = nodePorts.get(INPUT_1);
    auto in2 = nodePorts.get(INPUT_2);
    auto fac = nodePorts.get(INPUT_FAC);
    int option = options.at(MODE_DROPDOWN);

    cv::Mat result = math(fac, in1, in2);
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

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::UniMathOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Uni Math", "unimath", category);
        return builder.
                withOperator(std::make_unique<nitro::UniMathOperator>())->
                withIcon("math.png")->
                withNodeColor({36, 98, 131})->
                withDropDown(MODE_DROPDOWN, {"Add", "Subtract", "Multiply", "Divide", "Min", "Max", "Pow", "Log", "Abs",
                                             "Square Root"})->
                withInputValue(INPUT_FAC, 0, 0, 1)->
                withInputValue(INPUT_1, 0)->
                withInputValue(INPUT_2, 0)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
