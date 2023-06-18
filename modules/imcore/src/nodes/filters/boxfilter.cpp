#include "boxfilter.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_SIZE "Size"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"
#define BORDER_DROPDOWN "Border"

void nitro::BoxFilterOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    if (!nodePorts.inputsPresent({INPUT_IMAGE, INPUT_SIZE})) {
        return;
    }
    auto inputImg = nodePorts.getInputImage(INPUT_IMAGE);
    int kSize = nodePorts.getInputInteger(INPUT_SIZE);
    int option = options.at(MODE_DROPDOWN);
    int borderOption = options.at(BORDER_DROPDOWN);

    cv::Mat charImg;
    inputImg->convertTo(charImg, CV_8U, 255);

    cv::Mat resultChar;
    switch (option) {
        case 0:
            cv::blur(charImg, resultChar, cv::Size(kSize, kSize), cv::Point(-1, -1), borderOption);
            break;
        case 1:
            kSize = kSize % 2 == 0 ? std::max(kSize - 1, 1) : kSize;
            cv::medianBlur(charImg, resultChar, kSize);
            break;
        default:
            cv::blur(charImg, resultChar, cv::Size(kSize, kSize), cv::Point(-1, -1), borderOption);
            break;
    }

    cv::Mat result;
    resultChar.convertTo(result, CV_32F, 1.0 / 255.0);
    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::BoxFilterOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Box Filter", "boxFilter", category);
        return builder.
                withOperator(std::make_unique<nitro::BoxFilterOperator>())->
                withIcon("blur.png")->
                withNodeColor({71, 47, 189})->
                withDropDown(MODE_DROPDOWN, {"Average", "Median"})->
                withDropDown(BORDER_DROPDOWN, {"Constant", "Replicate", "Reflect"})->
                withInputImage(INPUT_IMAGE)->
                withInputInteger(INPUT_SIZE, 5, 1, 256)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
