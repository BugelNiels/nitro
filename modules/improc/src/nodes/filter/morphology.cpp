#include "morphology.hpp"
#include <nodes/nitronodebuilder.hpp>
#include <util.hpp>
#include <colimagedata.hpp>
#include <grayimagedata.hpp>

#include <opencv2/imgproc.hpp>
#include <QDebug>

namespace nitro::ImProc {

inline const QString INPUT_IMAGE_1 = "Image";
inline const QString INPUT_IMAGE_2 = "Kernel";
inline const QString OUTPUT_IMAGE = "Image";
inline const QString MODE_DROPDOWN = "Mode";

void MorphologyOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto im1 = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE_1);
    auto im2 = nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE_2);

    cv::Mat kernel;
    im2->convertTo(kernel, CV_8UC1, 255);
    int option = nodePorts.getOption(MODE_DROPDOWN);

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

    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<NitroNode>()> MorphologyOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Morphology", "morphology", category);
        return builder.
                withOperator(std::make_unique<MorphologyOperator>())->
                withIcon("morphology.png")->
                withNodeColor(NITRO_FILTER_COLOR)->
                withDropDown(MODE_DROPDOWN,
                             {"Dilate", "Erode", "Open", "Close", "Top Hat", "Black Hat"})->
                withInputPort<ColImageData>(INPUT_IMAGE_1)->
                withInputPort<GrayImageData>(INPUT_IMAGE_2)->
                withOutputPort<ColImageData>(OUTPUT_IMAGE)->
                build();
    };
}

} // namespace nitro::ImProc
