#include "morphology.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>
#include <QDebug>
#include "util.hpp"
#include "nodes/datatypes/imagedata.hpp"

#define INPUT_IMAGE_1 "Image"
#define INPUT_IMAGE_2 "Kernel"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

void nitro::MorphologyOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    auto im1 = nodePorts.inGet<ImageData>(INPUT_IMAGE_1).data();
    auto im2 = nodePorts.inGet<ImageData>(INPUT_IMAGE_2).data();


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

    nodePorts.output<ImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::MorphologyOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Morphology", "morphology", category);
        return builder.
                withOperator(std::make_unique<nitro::MorphologyOperator>())->
                withIcon("morphology.png")->
                withNodeColor({71, 47, 189})->
                withDropDown(MODE_DROPDOWN,
                             {"Dilate", "Erode", "Open", "Close", "Top Hat", "Black Hat"})->
                withInputPort<ImageData>(INPUT_IMAGE_1)->
                withInputPort<ImageData>(INPUT_IMAGE_2)->
                withOutputPort<ImageData>(OUTPUT_IMAGE)->
                build();
    };
}
