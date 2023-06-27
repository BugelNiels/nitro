#include "compress.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include "util.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_BITS "bits"
#define INPUT_SIZE "Size"
#define INPUT_APERTURE "Aperture"
#define OUTPUT_IMAGE "Residual"
#define OUTPUT_IMAGE_SMALL "Small"
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

void nitro::CompressOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }

    // Get the input data
    auto inputImg = *nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    int bits = nodePorts.inputInteger(INPUT_BITS);
    int levels = std::pow(2, bits);
    int size = nodePorts.inputInteger(INPUT_SIZE);

    double start = cv::getTickCount();

    // Color space convert
    cv::Mat uniformIm;
    cv::cvtColor(inputImg, uniformIm, cv::COLOR_GRAY2RGB);
    cvtColor(uniformIm, uniformIm, cv::COLOR_RGB2Lab);
    cv::Mat temp;
    uniformIm.convertTo(temp, CV_8U);
    temp.convertTo(uniformIm, CV_32F);
    std::vector<cv::Mat> channels;
    cv::split(uniformIm, channels);
    uniformIm = channels[0] / 100.0;

    // Small image
    cv::Mat centers;
    cv::Mat smallImg = nitro::resize(uniformIm, cv::Size(size, size), cv::INTER_LINEAR, AspectRatioMode::KEEP_GROW);
    smallImg = kMeansColors(smallImg, levels, 20, 0.0001, 1, centers);
    cv::Mat largeMain;
    cv::GaussianBlur(smallImg, largeMain, cv::Size(3, 3), 0, 0, cv::BorderTypes::BORDER_REFLECT);
    largeMain = nitro::resize(largeMain, uniformIm.size(), cv::INTER_LINEAR, AspectRatioMode::IGNORE);

    // Residual
    cv::Mat residual;
    cv::subtract(uniformIm, largeMain, residual);
    residual = kMeansColors(residual, levels, 20, 0.0001, 1, centers);
    residual = (residual + 1.0) / 2.0;
    double end = cv::getTickCount();
    // Store the result
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, residual);
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE_SMALL, smallImg);
    double elapsedTime = (end - start) / cv::getTickFrequency() * 1000.0;
    timeLabel_->setText(QString("Time: %1 msec").arg(elapsedTime));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::CompressOperator::creator(const QString &category) {
    return [category]() {
        auto *timeLabel = new QLabel("-");
        nitro::NitroNodeBuilder builder("Compress", "compress", category);
        return builder.
                withOperator(std::make_unique<nitro::CompressOperator>(timeLabel))->
                withIcon("blur.png")->
                withNodeColor(NITRO_COMPRESSION_COLOR)->
//                withDisplayWidget(TIME_LABEL, timeLabel)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_BITS, 3, 1, 8)->
                withInputInteger(INPUT_SIZE, 32, 3, 128, BoundMode::LOWER_ONLY)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE_SMALL)->
                build();
    };
}

nitro::CompressOperator::CompressOperator(QLabel *timeLabel) : timeLabel_(timeLabel) {}
