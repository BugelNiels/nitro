#include "compress.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include "util.hpp"
#include "../../../../imcore/src/nodes/compression/kmeans.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_BITS "bits"
#define INPUT_SIZE "Size"
#define INPUT_APERTURE "Aperture"
#define OUTPUT_IMAGE "Residual"
#define OUTPUT_IMAGE_SMALL "Small"
#define QUANTIZE_SMALL "Quantize small"
#define UNIFORM_LUM "Uniform Luminance"
#define TIME_LABEL "Time"

void nitro::CompressOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }

    // Get the input data
    auto inputImg = *nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    int bits = nodePorts.inputInteger(INPUT_BITS);
    int levels = std::pow(2, bits);
    float sizePortion = nodePorts.inputValue(INPUT_SIZE);

    double start = cv::getTickCount();

    // Color space convert
    cv::Mat uniformIm;
    if (nodePorts.optionEnabled(UNIFORM_LUM)) {
        cv::cvtColor(inputImg, uniformIm, cv::COLOR_GRAY2RGB);
        cvtColor(uniformIm, uniformIm, cv::COLOR_RGB2Lab);
        cv::Mat temp;
        uniformIm.convertTo(temp, CV_8U);
        temp.convertTo(uniformIm, CV_32F);
        std::vector<cv::Mat> channels;
        cv::split(uniformIm, channels);
        uniformIm = channels[0] / 100.0;
    } else {
        inputImg.copyTo(uniformIm);
    }

    // Small image
    cv::Mat residual;
    cv::Mat smallImg;
    if (sizePortion > 0) {
        int smallWidth = sizePortion * uniformIm.cols + 0.5;
        int smallHeight = sizePortion * uniformIm.rows + 0.5;
        cv::resize(uniformIm, smallImg, {smallWidth, smallHeight});

        if (nodePorts.optionEnabled(QUANTIZE_SMALL)) {
            smallImg = nitro::kMeansHist(smallImg, levels, 40);
        }

        cv::Mat largeMain;
        cv::GaussianBlur(smallImg, largeMain, cv::Size(3, 3), 0, 0, cv::BorderTypes::BORDER_REFLECT);
        cv::resize(largeMain, largeMain, uniformIm.size());

        // Residual
        cv::subtract(uniformIm, largeMain, residual);
        nodePorts.output<GrayImageData>(OUTPUT_IMAGE_SMALL, smallImg);
    } else {
        residual = uniformIm;
        smallImg = cv::Mat(1, 1, CV_32F, cv::Scalar(0));
    }
    residual = nitro::kMeansHist((residual + 1.0) / 2.0, levels, 40);
    double end = cv::getTickCount();
    // Store the result
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE_SMALL, smallImg);
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, residual);
    double elapsedTime = (end - start) / cv::getTickFrequency() * 1000.0;
    timeLabel_->setText(QString("Time: %1 msec").arg(elapsedTime));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::CompressOperator::creator(const QString &category) {
    return [category]() {
        auto *timeLabel = new QLabel("-");
        nitro::NitroNodeBuilder builder("Bit Compress", "bitCompress", category);
        return builder.
                withOperator(std::make_unique<nitro::CompressOperator>(timeLabel))->
                withIcon("compress.png")->
                withNodeColor(NITRO_COMPRESSION_COLOR)->
                withDisplayWidget(TIME_LABEL, timeLabel)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_BITS, 3, 1, 8)->
                withInputValue(INPUT_SIZE, 0.2, 0, 1, BoundMode::UPPER_LOWER)->
                withCheckBox(QUANTIZE_SMALL, false)->
                withCheckBox(UNIFORM_LUM, false)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE_SMALL)->
                build();
    };
}

nitro::CompressOperator::CompressOperator(QLabel *timeLabel) : timeLabel_(timeLabel) {}
