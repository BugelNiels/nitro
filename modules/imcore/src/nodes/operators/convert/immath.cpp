#include "immath.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>


void nitro::MathOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {
    bool im1Present, facPresent;
    auto im1 = nodePorts.getInputImage("Image", im1Present);
    double fac = nodePorts.getInputValue("Fac", facPresent);

    int option = options.at("Mode");

    if (!im1Present || !facPresent) {
        return;
    }
    cv::Mat result;
    switch (option) {
        case 0: {
            cv::Mat maxValueMat(im1->size(), im1->type(), cv::Scalar(fac * getMaxValue(*im1)));
            cv::add(*im1, maxValueMat, result);
            break;
        }
        case 1: {
            cv::Mat maxValueMat(im1->size(), im1->type(), cv::Scalar(getMaxValue(*im1)));
            cv::subtract(*im1, fac * maxValueMat, result);
            break;
        }
        case 2:
            result = *im1 * fac;
            break;
        case 3:
            result = *im1 / fac;
            break;
        default:
            result = *im1 * fac;
            break;

    }

    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::MathOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Math", "math", category);
        return builder.
                withOperator(std::make_unique<nitro::MathOperator>())->
                withIcon(":/icons/nodes/math.png")->
                withNodeColor({110, 110, 29})->
                withDropDown("Mode", {"Add", "Subtract", "Multiply", "Divide"})->
                withInputImage("Image")->
                withInputValue("Value", 0.5, 0, 1)->
                withOutputImage("Image")->
                build();
    };
}
