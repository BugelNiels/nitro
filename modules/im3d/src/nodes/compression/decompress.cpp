#include "decompress.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include "util.hpp"
#include "nodes/restoration/resample.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Residual"
#define INPUT_SMALL "Small"
#define OUTPUT_IMAGE "Image"
#define UNIFORM_LUM "Uniform Luminance"
#define TIME_LABEL "Time"

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
    cv::Mat result = (resampleImage(residual, true) * 2.0) - 1.0 + largeMain;

    if (nodePorts.optionEnabled(UNIFORM_LUM)) {
        result = toRgb(result);
    }

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
                withIcon("expand.png")->
                withNodeColor(NITRO_COMPRESSION_COLOR)->
                withDisplayWidget(TIME_LABEL, timeLabel)->
                withCheckBox(UNIFORM_LUM, false)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withInputPort<GrayImageData>(INPUT_SMALL)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                build();
    };
}

nitro::DecompressOperator::DecompressOperator(QLabel *timeLabel) : timeLabel_(timeLabel) {}
