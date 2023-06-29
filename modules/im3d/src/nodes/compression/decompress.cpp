#include "decompress.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include "util.hpp"
#include "nodes/restoration/resample.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Residual"
#define INPUT_SMALL "Small"
#define OUTPUT_IMAGE "Image"
#define TIME_LABEL "Time"

static cv::Mat
kMeansColors(const cv::Mat &image, int numColors, int maxIter, double epsilon, int maxAttempts, cv::Mat &centers) {
    cv::Mat labels;
    cv::Mat samples = image.reshape(1, image.rows * image.cols);
    if (samples.rows < numColors) {
        return {};
    }
    cv::kmeans(samples,
               numColors,
               labels,
               cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, maxIter, epsilon),
               maxAttempts,
               cv::KMEANS_RANDOM_CENTERS, centers);

    cv::Mat quantImg(image.size(), image.type());
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            int cluster_idx = labels.at<int>(y * image.cols + x, 0);
            quantImg.at<float>(y, x) = centers.at<float>(cluster_idx, 0);
        }
    }
    return quantImg;
}

static cv::Mat toRgb(const cv::Mat &img) {
    cv::Mat result;
    cv::cvtColor(img, result, cv::COLOR_GRAY2RGB);
    std::vector<cv::Mat> channels;
    cv::split(result, channels);
    channels[0] = channels[0] * 100.0;
    cv::merge(channels, result);
    cvtColor(result, result, cv::COLOR_Lab2RGB);
    cv::split(result, channels);
    return channels[0];
}

void nitro::DecompressOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }

    // Get the input data
    auto residual = *nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    auto smallImg = *nodePorts.inGetAs<GrayImageData>(INPUT_SMALL);

    double start = cv::getTickCount();

    // Small image
    cv::Mat largeMain;
    cv::GaussianBlur(smallImg, largeMain, cv::Size(3, 3), 0, 0, cv::BorderTypes::BORDER_REFLECT);
    largeMain = nitro::resize(largeMain, residual.size(), cv::INTER_LINEAR, AspectRatioMode::IGNORE);

    // Residual
    cv::Mat resampled = resampleImage(residual, 0, 8, true);
    resampled = (resampled * 2.0) - 1.0;

    cv::add(resampled, largeMain, resampled);

    // Color space convert
    cv::Mat result = toRgb(resampled);

    double end = cv::getTickCount();
    // Store the result
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
    double elapsedTime = (end - start) / cv::getTickFrequency() * 1000.0;
    timeLabel_->setText(QString("Time: %1 msec").arg(elapsedTime));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::DecompressOperator::creator(const QString &category) {
    return [category]() {
        auto *timeLabel = new QLabel("-");
        nitro::NitroNodeBuilder builder("Bit Decompress", "bitDecompress", category);
        return builder.
                withOperator(std::make_unique<nitro::DecompressOperator>(timeLabel))->
                withIcon("blur.png")->
                withNodeColor(NITRO_COMPRESSION_COLOR)->
//                withDisplayWidget(TIME_LABEL, timeLabel)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withInputPort<GrayImageData>(INPUT_SMALL)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                build();
    };
}

nitro::DecompressOperator::DecompressOperator(QLabel *timeLabel) : timeLabel_(timeLabel) {}
