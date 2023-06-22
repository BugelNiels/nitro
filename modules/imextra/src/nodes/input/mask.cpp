#include "mask.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_MASK_WIDTH "Scale X"
#define INPUT_MASK_HEIGHT "Scale Y"
#define INPUT_X "Width"
#define INPUT_Y "Height"
#define INPUT_POS_X "X"
#define INPUT_POS_Y "Y"
#define OUTPUT_IMAGE "Mask"
#define MODE_DROPDOWN "Mode"

// TODO: rotation
void nitro::MaskOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    int option = options.at(MODE_DROPDOWN);
    int width = nodePorts.inputInteger(INPUT_X);
    int height = nodePorts.inputInteger(INPUT_Y);
    double innerWidth = nodePorts.inputValue(INPUT_MASK_WIDTH);
    double innerHeight = nodePorts.inputValue(INPUT_MASK_HEIGHT);
    double posX = nodePorts.inputValue(INPUT_POS_X);
    double posY = nodePorts.inputValue(INPUT_POS_Y);

    cv::MorphShapes shape;
    switch (option) {
        case 0:
            shape = cv::MORPH_ELLIPSE;
            break;
        case 1:
            shape = cv::MORPH_RECT;
            break;
        default:
            shape = cv::MORPH_ELLIPSE;
            break;
    }
    int kernelWidth = int(std::round(innerWidth * width / 2.0));
    int kernelHeight = int(std::round(innerHeight * height / 2.0));
    cv::Mat image(height, width, CV_8UC1, cv::Scalar(0));
    if (kernelHeight > 0 && kernelWidth > 0) {
        int centerX = int(std::round(width * posX));
        int centerY = int(std::round(height * posY));
        if(option == 0) {
            cv::ellipse(image, {centerX, centerY}, {kernelWidth, kernelHeight}, 0, 0, 360, cv::Scalar(255), -1);
        } else {
            cv::Point v1 = {centerX - kernelWidth, centerY - kernelHeight};
            cv::Point v2 = {centerX + kernelWidth, centerY + kernelHeight};

            cv::rectangle(image, v1, v2, cv::Scalar(255), -1);
        }


    }

    cv::Mat result;
    image.convertTo(result, CV_32F);

    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::MaskOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Mask", "mask", category);
        return builder.
                withOperator(std::make_unique<nitro::MaskOperator>())->
                withIcon("mask.png")->
                withNodeColor(NITRO_INPUT_COLOR)->
                withDropDown(MODE_DROPDOWN, {"Ellipse", "Rectangle"})->
                withInputInteger(INPUT_X, 256, 1, 4096)->
                withInputInteger(INPUT_Y, 256, 1, 4096)->
                withInputValue(INPUT_MASK_WIDTH, 1, 0, 2, BoundMode::LOWER_ONLY)->
                withInputValue(INPUT_MASK_HEIGHT, 1, 0, 2, BoundMode::LOWER_ONLY)->
                withInputValue(INPUT_POS_X, 0.5, 0, 1, BoundMode::UNCHECKED)->
                withInputValue(INPUT_POS_Y, 0.5, 0, 1, BoundMode::UNCHECKED)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                build();
    };
}
