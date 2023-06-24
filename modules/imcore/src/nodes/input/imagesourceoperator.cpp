#include "imagesourceoperator.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include "util.hpp"
#include "nodes/datatypes/grayimagedata.hpp"

#include <QDebug>
#include <opencv2/imgcodecs.hpp>
#include <QImageReader>

#define OUTPUT_IMAGE "Image"
#define DISPLAY_IMAGE "Display Img"
#define OUTPUT_ALPHA "Alpha"

nitro::ImageSourceOperator::ImageSourceOperator(QLabel *displayImgLabel) : displayImgLabel_(displayImgLabel) {}

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

void nitro::ImageSourceOperator::execute(NodePorts &nodePorts) {
    QString filePath = nodePorts.getGlobalProperty(OUTPUT_IMAGE);
    cv::Mat img;
    cv::Mat alpha;
    loadImage(filePath, img, alpha);
    if (img.empty()) {
        img = blankImg_;
        nodePorts.output<ColImageData>(OUTPUT_IMAGE, blankImg_);
        nodePorts.output<GrayImageData>(OUTPUT_ALPHA, blankImg_);
    } else {
        nodePorts.output<ColImageData>(OUTPUT_IMAGE, img);
        if (alpha.empty()) {
            alpha = {img.rows, img.cols, CV_32FC1, cv::Scalar(0)};
            nodePorts.output<GrayImageData>(OUTPUT_ALPHA, alpha);
        } else {
            nodePorts.output<GrayImageData>(OUTPUT_ALPHA, alpha);
        }
    }
    int size = displayImgLabel_->width();
    displayImgLabel_->setFixedHeight(size);
    displayImgLabel_->setPixmap(
            QPixmap::fromImage(cvMatToQImage(img, displayBuf_))
                    .scaled(size, size, Qt::KeepAspectRatio));

}

std::function<std::unique_ptr<nitro::NitroNode>()>
nitro::ImageSourceOperator::creator(const QString &category) {

    return [category]() {
        NitroNodeBuilder builder("Image Source", "ImageSource", category);

        auto *imgDisplayLabel = new QLabel();
        imgDisplayLabel->setAlignment(Qt::AlignCenter);
        imgDisplayLabel->setStyleSheet("border: 1px solid grey;");
        return builder.
                withOperator(std::make_unique<nitro::ImageSourceOperator>(imgDisplayLabel))->
                withLoadButton(OUTPUT_IMAGE, "Img Files (*.png *.jpg *.jpeg *.tiff *.tif *pgm *ppm)")->
                withDisplayWidget(DISPLAY_IMAGE, imgDisplayLabel)->
                withIcon("image_source.png")->
                withNodeColor(NITRO_IMAGE_COLOR)->
                withOutputPort<GrayImageData>(OUTPUT_ALPHA)->
                build();
    };
}

