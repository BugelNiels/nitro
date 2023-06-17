#include "morphology.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>
#include <QDebug>
#include "util.hpp"

#define INPUT_IMAGE_1 "Image"
#define INPUT_IMAGE_2 "Kernel"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

void nitro::MorphologyOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {
    if (!nodePorts.inputsPresent({INPUT_IMAGE_1, INPUT_IMAGE_2})) {
        return;
    }
    auto im1 = nodePorts.getInputImage(INPUT_IMAGE_1);
    auto im2 = nodePorts.getInputImage(INPUT_IMAGE_2);


    cv::Mat kernel;
    im2->convertTo(kernel, CV_8UC1, 255);

    int option = options.at(MODE_DROPDOWN);

    cv::Mat result;
    switch (option) {
        case 0:
            cv::dilate(*im1, result, kernel);
            break;
        case 1:
            cv::erode(*im1, result, kernel);
            break;
        case 2:
            cv::morphologyEx(*im1, result, cv::MORPH_OPEN, kernel);
            break;
        case 3:
            cv::morphologyEx(*im1, result, cv::MORPH_CLOSE, kernel);
            break;
        case 4:
            cv::morphologyEx(*im1, result, cv::MORPH_TOPHAT, kernel);
            break;
        case 5:
            cv::morphologyEx(*im1, result, cv::MORPH_BLACKHAT, kernel);
            break;
        default:
            cv::dilate(*im1, result, kernel);
            break;
    }

    result.convertTo(result, CV_32F);

    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::MorphologyOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Morphology", "morphology", category);
        return builder.
                withOperator(std::make_unique<nitro::MorphologyOperator>())->
                withIcon("mix.png")->
                withNodeColor({71, 47, 189})->
                withDropDown(MODE_DROPDOWN,
                             {"Dilate", "Erode", "Open", "Close", "Top Hat", "Black Hat"})->
                withInputImage(INPUT_IMAGE_1)->
                withInputImage(INPUT_IMAGE_2)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
