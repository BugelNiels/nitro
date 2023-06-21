#include "mask.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_MASK_WIDTH "Inner Width"
#define INPUT_MASK_HEIGHT "Inner Height"
#define INPUT_WIDTH "Width"
#define INPUT_HEIGHT "Height"
#define OUTPUT_IMAGE "Kernel"
#define MODE_DROPDOWN "Mode"

void nitro::MaskOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    int option = options.at(MODE_DROPDOWN);
    int width = nodePorts.inputInteger(INPUT_WIDTH);
    int height = nodePorts.inputInteger(INPUT_HEIGHT);
    double innerWidth = nodePorts.inputValue(INPUT_MASK_WIDTH);
    double innerHeight = nodePorts.inputValue(INPUT_MASK_HEIGHT);

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
    int kernelWidth = int(std::round(innerWidth * width));
    int kernelHeight = int(std::round(innerHeight * height));
    cv::Mat image(height, width, CV_8UC1, cv::Scalar(0));
    if (kernelHeight > 0 && kernelWidth > 0) {
        cv::Mat kernel = cv::getStructuringElement(shape, {kernelWidth, kernelHeight});

        int copyWidth = std::min(width, kernelWidth);
        int copyHeight = std::min(height, kernelHeight);
        int kernelOffsetX = kernelWidth > width ? (kernelWidth - width) / 2 : 0;
        int kernelOffsetY = kernelHeight > height ? (kernelHeight - height) / 2 : 0;
        int offsetX = width > kernelWidth ? (width - kernelWidth) / 2 : 0;
        int offsetY = height > kernelHeight ? (height - kernelHeight) / 2 : 0;

        cv::Rect kernRoi(kernelOffsetX, kernelOffsetY, copyWidth, copyHeight);
        cv::Rect imRoi(offsetX, offsetY, copyWidth, copyHeight);
        kernel(kernRoi).copyTo(image(imRoi));

    }

    cv::Mat result;
    image.convertTo(result, CV_32F);

    nodePorts.output<ImageData>(OUTPUT_IMAGE, result);
}

// TODO: position
std::function<std::unique_ptr<nitro::NitroNode>()> nitro::MaskOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Mask", "mask", category);
        return builder.
                withOperator(std::make_unique<nitro::MaskOperator>())->
                withIcon("mask.png")->
                withNodeColor(NITRO_INPUT_COLOR)->
                withDropDown(MODE_DROPDOWN, {"Ellipse", "Rectangle"})->
                withInputInteger(INPUT_WIDTH, 256, 1, 4096)->
                withInputInteger(INPUT_HEIGHT, 256, 1, 4096)->
                withInputValue(INPUT_MASK_WIDTH, 1, 0, 2)->
                withInputValue(INPUT_MASK_HEIGHT, 1, 0, 2)->
                withOutputPort<ImageData>(OUTPUT_IMAGE)->
                build();
    };
}
