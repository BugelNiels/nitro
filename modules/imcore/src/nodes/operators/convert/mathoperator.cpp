#include "mathoperator.hpp"
#include <opencv2/imgproc.hpp>
#include <QDebug>

int getMaxValue(const cv::Mat &mat) {
    int depth = mat.depth();

    int maxValue;

    switch (depth) {
        case CV_8U:  // 8-bit unsigned integer (0-255)
            maxValue = 255;
            break;

        case CV_8S:  // 8-bit signed integer (-128 to 127)
            maxValue = 127;
            break;

        case CV_16U:  // 16-bit unsigned integer (0-65535)
            maxValue = 65535;
            break;

        case CV_16S:  // 16-bit signed integer (-32768 to 32767)
            maxValue = 32767;
            break;

        case CV_32S:  // 32-bit signed integer
            maxValue = INT_MAX;
            break;

        case CV_32F:  // 32-bit floating-point
            maxValue = 1.0f;
            break;

        case CV_64F:  // 64-bit floating-point
            maxValue = 1.0;
            break;

        default:
            maxValue = 0;
            break;
    }
    return maxValue;
}

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
