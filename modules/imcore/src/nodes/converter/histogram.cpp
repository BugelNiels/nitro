#include "histogram.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include <opencv2/imgproc.hpp>

#include <QDebug>

#define INPUT_IMAGE "Image"
#define OUTPUT_HIST "Histogram"
#define HIST_DISPLAY "Histogram display"

#define NUM_BINS 256

cv::Mat createHistogramImage(const cv::Mat &histogram) {
    int histSize = histogram.rows;
    int imageSize = NUM_BINS;
    int binWidth = cvRound(static_cast<double>(imageSize) / histSize);
    cv::Mat histImage(imageSize, imageSize, CV_32FC1, cv::Scalar(0));

    cv::Mat normalizedHist;
    cv::normalize(histogram, normalizedHist, 0, imageSize, cv::NORM_MINMAX, CV_32F);

    for (int i = 0; i < histSize; i++) {
        float histValue = normalizedHist.at<float>(i);
        int barHeight = cvRound(histValue);
        cv::rectangle(histImage, cv::Point(i * binWidth, imageSize - 1),
                      cv::Point((i + 1) * binWidth - 1, imageSize - barHeight), cv::Scalar(1), cv::FILLED);
    }

    return histImage;
}

static cv::Mat getHistogram(const cv::Mat &src) {
    cv::Mat histogram;
    int numBins = NUM_BINS;
    float range[] = {0, 1};
    const float *histRange = {range};

    cv::calcHist(&src, 1, nullptr, cv::Mat(), histogram, 1, &numBins, &histRange);
    return histogram;
}

void nitro::HistogramOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto img = *nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    cv::Mat histogram = getHistogram(img);

    cv::Mat histImg = createHistogramImage(histogram);

    histLabel_->setPixmap(QPixmap::fromImage(cvMatToQImage(histImg, displayImage_)).scaled(150, 20, Qt::AspectRatioMode::IgnoreAspectRatio,
                                                                                           Qt::SmoothTransformation));

    nodePorts.output<GrayImageData>(OUTPUT_HIST, histImg);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::HistogramOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Histogram", "histogram", category);
        auto *histLabel = new QLabel();
        return builder
                .withOperator(std::make_unique<nitro::HistogramOperator>(histLabel))->
                withIcon("hist.png")->
                withNodeColor(NITRO_CONVERTER_COLOR)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withDisplayWidget(HIST_DISPLAY, histLabel)->
                withOutputPort<GrayImageData>(OUTPUT_HIST)->
                build();
    };
}

nitro::HistogramOperator::HistogramOperator(QLabel *histLabel) : histLabel_(histLabel) {}

