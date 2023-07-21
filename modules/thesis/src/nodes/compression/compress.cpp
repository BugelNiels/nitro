#include "compress.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "../../../../imcore/include/grayimagedata.hpp"
#include "util.hpp"
#include "../../../../improc/src/nodes/quantization/kmeans.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include <opencv2/imgproc.hpp>

namespace nitro::Thesis {

inline const QString INPUT_IMAGE = "Image";
inline const QString INPUT_BITS = "bits";
inline const QString INPUT_SIZE = "Size";
inline const QString OUTPUT_IMAGE = "Residual";
inline const QString OUTPUT_IMAGE_SMALL = "Small";
inline const QString QUANTIZE_SMALL = "Quantize small";
inline const QString UNIFORM_LUM = "Uniform Luminance";
inline const QString TIME_LABEL = "Time";

void CompressOperator::execute(NodePorts &nodePorts) {
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
            smallImg = nitro::ImProc::kMeansHist(smallImg, levels, 40);
        }

        cv::Mat largeMain;
        cv::GaussianBlur(smallImg, largeMain, cv::Size(3, 3), 0, 0, cv::BorderTypes::BORDER_REFLECT);
        cv::resize(largeMain, largeMain, uniformIm.size());

        // Residual
        cv::subtract(uniformIm, largeMain, residual);
    } else {
        residual = uniformIm;
        smallImg = cv::Mat(1, 1, CV_32F, cv::Scalar(0));
    }
    residual = nitro::ImProc::kMeansHist((residual + 1.0) / 2.0, levels, 40);
    double end = cv::getTickCount();
    double elapsedTime = (end - start) / cv::getTickFrequency() * 1000.0;
    timeLabel_->setText(QString("Time: %1 msec").arg(elapsedTime));
    // Store the result
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE_SMALL, smallImg);
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, residual);
}

std::function<std::unique_ptr<NitroNode>()> CompressOperator::creator(const QString &category) {
    return [category]() {
        auto *timeLabel = new QLabel("-");
        NitroNodeBuilder builder("Bit Compress", "bitCompress", category);
        return builder.
                withOperator(std::make_unique<CompressOperator>(timeLabel))->
                withIcon("compress.png")->
                withNodeColor(NITRO_COMPRESSION_COLOR)->
                withDisplayWidget(TIME_LABEL, timeLabel)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_BITS, 3, 1, 8)->
                withInputValue(INPUT_SIZE, 0.125, 0, 1, BoundMode::UPPER_LOWER)->
                withCheckBox(QUANTIZE_SMALL, false)->
                withCheckBox(UNIFORM_LUM, false)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE_SMALL)->
                build();
    };
}

CompressOperator::CompressOperator(QLabel *timeLabel) : timeLabel_(timeLabel) {}


} // namespace nitro::Thesis
