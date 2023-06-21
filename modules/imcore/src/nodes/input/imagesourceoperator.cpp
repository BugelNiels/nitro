#include "imagesourceoperator.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include "util.hpp"

#include <QDebug>
#include <opencv2/imgcodecs.hpp>
#include <QImageReader>

#define OUTPUT_IMAGE "Image"
#define OUTPUT_ALPHA "Alpha"

void loadImage(const QString &filePath, cv::Mat &dest, cv::Mat &alpha) {
    cv::Mat inputImg = cv::imread(filePath.toStdString(), -1);
    if (inputImg.empty()) {
        return;
    }

    if (inputImg.channels() == 3 && nitro::isGrayscale(inputImg)) {
        cv::Mat gray;
        cvtColor(inputImg, gray, cv::COLOR_RGB2GRAY);
        inputImg = gray;
    } else if (inputImg.channels() == 3) {
        cv::cvtColor(inputImg, inputImg, cv::COLOR_BGR2RGB);
    } else if (inputImg.channels() == 4) {
        std::vector<cv::Mat> channels;
        cv::split(inputImg, channels);
        alpha = channels[3];
        alpha.convertTo(alpha, CV_32F, 1.0 / nitro::getMaxValue(alpha));
        cv::cvtColor(inputImg, inputImg, cv::COLOR_BGRA2RGB);
    }
    inputImg.convertTo(dest, CV_32F, 1.0 / nitro::getMaxValue(inputImg));
}

void nitro::ImageSourceOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    QString filePath = nodePorts.getGlobalProperty(OUTPUT_IMAGE);
    cv::Mat img;
    cv::Mat alpha;
    loadImage(filePath, img, alpha);
    if (img.empty()) {
        nodePorts.output<ImageData>(OUTPUT_IMAGE, blankImg_);
        nodePorts.output<ImageData>(OUTPUT_ALPHA, blankImg_);
    } else {
        nodePorts.output<ImageData>(OUTPUT_IMAGE, img);
        if (alpha.empty()) {
            alpha = {img.rows, img.cols, CV_32FC1, cv::Scalar(0)};
            nodePorts.output<ImageData>(OUTPUT_ALPHA, alpha);
        } else {
            nodePorts.output<ImageData>(OUTPUT_ALPHA, alpha);
        }
    }

}

std::function<std::unique_ptr<nitro::NitroNode>()>
nitro::ImageSourceOperator::creator(const QString &category) {

    return [category]() {
        NitroNodeBuilder builder("Image Source", "ImageSource", category);


        return builder.
                withOperator(std::make_unique<nitro::ImageSourceOperator>())->
                withLoadButton(OUTPUT_IMAGE, "Img Files (*.png *.jpg *.jpeg *.tiff *.tif *pgm *ppm)")->
                withOutputPort<ImageData>(OUTPUT_ALPHA)->
                withIcon("image_source.png")->
                withNodeColor(NITRO_IMAGE_COLOR)->
                build();
    };
}
