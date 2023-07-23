#include "imagesourceoperator.hpp"
#include "include/colimagedata.hpp"
#include "include/grayimagedata.hpp"
#include <nodes/nitronodebuilder.hpp>
#include <util.hpp>

#include <QImageReader>
#include <opencv2/imgcodecs.hpp>

namespace nitro::ImCore {

static inline const QString OUTPUT_IMAGE = "Image";
static inline const QString DISPLAY_IMAGE = "Display Img";
static inline const QString OUTPUT_ALPHA = "Alpha";

ImageSourceOperator::ImageSourceOperator(QLabel *displayImgLabel)
    : displayImgLabel_(displayImgLabel) {}

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

void ImageSourceOperator::execute(NodePorts &nodePorts) {
    QString filePath = nodePorts.getGlobalProperty(OUTPUT_IMAGE);
    cv::Mat img;
    cv::Mat alpha;
    loadImage(filePath, img, alpha);
    if (img.empty()) {
        img = blankImg_;
        nodePorts.output<ColImageData>(OUTPUT_IMAGE, blankImg_);
        nodePorts.output<GrayImageData>(OUTPUT_ALPHA, blankImg_);
    } else {
        if (isGrayscale(img)) {

            nodePorts.output<GrayImageData>(OUTPUT_IMAGE, img);
        } else {

            nodePorts.output<ColImageData>(OUTPUT_IMAGE, img);
        }
        if (alpha.empty()) {
            alpha = {img.rows, img.cols, CV_32FC1, cv::Scalar(0)};
            nodePorts.output<GrayImageData>(OUTPUT_ALPHA, alpha);
        } else {
            nodePorts.output<GrayImageData>(OUTPUT_ALPHA, alpha);
        }
    }
    int size = displayImgLabel_->width();
    displayImgLabel_->setFixedHeight(size);
    displayImgLabel_->setPixmap(QPixmap::fromImage(cvMatToQImage(img, displayBuf_))
                                        .scaled(size, size, Qt::KeepAspectRatio));
}

std::function<std::unique_ptr<NitroNode>()> ImageSourceOperator::creator(const QString &category) {

    return [category]() {
        auto *imgDisplayLabel = new QLabel();
        imgDisplayLabel->setAlignment(Qt::AlignCenter);
        imgDisplayLabel->setStyleSheet("border: 1px solid grey;");
        NitroNodeBuilder builder("Image Source", "ImageSource", category);
        return builder.withOperator(std::make_unique<ImageSourceOperator>(imgDisplayLabel))
                ->withLoadButton(OUTPUT_IMAGE,
                                 "Img Files (*.png *.jpg *.jpeg *.tiff *.tif *pgm *ppm)")
                ->withDisplayWidget(DISPLAY_IMAGE, imgDisplayLabel)
                ->withIcon("image_source.png")
                ->withNodeColor(NITRO_IMAGE_COLOR)
                ->withOutputPort<GrayImageData>(OUTPUT_ALPHA)
                ->build();
    };
}

} // namespace nitro::ImCore
