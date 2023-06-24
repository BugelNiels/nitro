#include "colormap.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include <opencv2/imgproc.hpp>

#include <QDebug>

#define INPUT_IMAGE "Image"
#define OPTION_DROPDOWN "Option"
#define DISPLAY_LABEL "Color Label"
#define OUTPUT_IMAGE "Image"

static cv::Mat createGradientImage(int width, int height) {
    cv::Mat gradientImage(height, width, CV_8UC1);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            gradientImage.at<uchar>(y, x) = static_cast<uchar>(255 * x / width);
        }
    }

    return gradientImage;
}

nitro::ColorMapOperator::ColorMapOperator(QLabel *displayLabel) : displayLabel_(displayLabel) {}

void nitro::ColorMapOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }

    int option = nodePorts.getOption(OPTION_DROPDOWN);
    auto colormapType = static_cast<cv::ColormapTypes>(option);
    cv::Mat mapLabel = createGradientImage(200, 20);
    cv::applyColorMap(mapLabel, mapLabel, colormapType);
    displayLabel_->setPixmap(QPixmap::fromImage(cvMatToQImage(mapLabel, displayImage_)));

    auto img = nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);;
    cv::Mat result;

    cv::Mat imIn;
    img->convertTo(imIn, CV_8U, 255);

    cv::applyColorMap(imIn, result, colormapType);
    result.convertTo(result, CV_32F, 1 / 255.0);


    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ColorMapOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Apply Color Map", "colorMap", category);
        auto *displayLabel = new QLabel();
        return builder
                .withOperator(std::make_unique<nitro::ColorMapOperator>(displayLabel))->
                withIcon("colormap.png")->
                withNodeColor(NITRO_COLOR_COLOR)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withDropDown(OPTION_DROPDOWN,
                             {
                                     "Autumn",
                                     "Bone",
                                     "Jet",
                                     "Winter",
                                     "Rainbow",
                                     "Ocean",
                                     "Summer",
                                     "Spring",
                                     "Cool",
                                     "Hsv",
                                     "Pink",
                                     "Hot",
                                     "Parula",
                                     "Magma",
                                     "Inferno",
                                     "Plasma",
                                     "Viridis",
                                     "Cividis",
                                     "Twilight",
                                     "Twilight Shifted",
                                     "Turbo",
                                     "Deep green"
                             })->
                withDisplayWidget(DISPLAY_LABEL, displayLabel)->
                withOutputPort<ColImageData>(OUTPUT_IMAGE)->
                build();
    };
}

