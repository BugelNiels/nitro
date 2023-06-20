#include "imagesourceoperator.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include "util.hpp"

#include <QDebug>
#include <opencv2/imgcodecs.hpp>
#include <QImageReader>

#define OUTPUT_IMAGE "Image"

cv::Mat loadImage(const QString &filePath) {
    cv::Mat inputImg = cv::imread(filePath.toStdString());
    if(inputImg.empty()) {
        return inputImg;
    }

    if(inputImg.channels() == 1 && nitro::isGrayscale(inputImg)) {
        cv::Mat gray;
        cvtColor(inputImg, gray, cv::COLOR_RGB2GRAY);
        inputImg = gray;
    } else if (inputImg.channels() == 3) {

        cv::cvtColor(inputImg, inputImg, cv::COLOR_BGR2RGB);
    } else if(inputImg.channels() == 4) {
        cv::cvtColor(inputImg, inputImg, cv::COLOR_BGRA2RGB);
        // Remove the alpha channel for now
    }

    cv::Mat floatImg;
    float max = nitro::getMaxValue(inputImg);
    inputImg.convertTo(floatImg, CV_32F, 1.0 / max);
    inputImg = floatImg;
    return floatImg;
}

void nitro::ImageSourceOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    QString filePath = nodePorts.getGlobalProperty(OUTPUT_IMAGE);
    cv::Mat img = loadImage(filePath);
    if (img.empty()) {
        nodePorts.output<ImageData>(OUTPUT_IMAGE, blankImg_);
    } else {
        nodePorts.output<ImageData>(OUTPUT_IMAGE, img);
    }

}

std::function<std::unique_ptr<nitro::NitroNode>()>
nitro::ImageSourceOperator::creator(const QString &category) {

    return [category]() {
        NitroNodeBuilder builder("Image Source", "ImageSource", category);


        return builder.
                withOperator(std::make_unique<nitro::ImageSourceOperator>())->
                withLoadButton(OUTPUT_IMAGE, "Img Files (*.png *.jpg *.jpeg *.tiff *.tif *pgm *ppm)")->
                withIcon("image_source.png")->
                withNodeColor({121, 70, 29})->
                build();
    };
}
