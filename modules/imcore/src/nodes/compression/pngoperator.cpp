#include "pngoperator.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include "nodes/datatypes/grayimagedata.hpp"

#define DISPLAY_LABEL_COMP "compLabel"
#define DISPLAY_LABEL_ORIG "origLabel"
#define DISPLAY_LABEL_RATIO "ratioLabel"
#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Compressed Image"
#define INPUT_QUALITY "Compression Lvl"

#define OUTPUT_COMP_SIZE "Compressed"
#define OUTPUT_ORIG_SIZE "Original"
#define OUTPUT_RATIO "Ratio"

#include <QDebug>

#include <zlib.h>
#include <opencv2/imgcodecs.hpp>


nitro::PngOperator::PngOperator(QLabel *valueLabel, QLabel *originalSizeLabel,
                                  QLabel *ratioLabel)
        : valueLabel_(valueLabel), originalSizeLabel_(originalSizeLabel), ratioLabel_(ratioLabel) {}

void nitro::PngOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto img = *nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    int quality = nodePorts.inputInteger(INPUT_QUALITY);

    cv::Mat data;
    img.convertTo(data, CV_8U, 255);


    cv::Mat result;
    std::vector<int> compression_params = {cv::IMWRITE_PNG_COMPRESSION, quality};
    std::vector<uchar> png_buffer;
    cv::imencode(".png", data, png_buffer, compression_params);
    unsigned long size = png_buffer.size();
    img.copyTo(result); // Lossless compression

    double compressKb = size / 1000.0;
    double originalKb = data.total() * data.elemSize() / 1000.0;

    QString sizeString = QString("Input: %1 KB").arg(originalKb);
    QString compressSizeString = QString("Compressed: %1 KB").arg(compressKb);
    QString ratioString = QString("Ratio: %1").arg(QString::number(originalKb / compressKb, 'f', 3));

    originalSizeLabel_->setText(sizeString);
    valueLabel_->setText(compressSizeString);
    ratioLabel_->setText(ratioString);

    nodePorts.output<DecimalData>(OUTPUT_COMP_SIZE, compressKb);
    nodePorts.output<DecimalData>(OUTPUT_ORIG_SIZE, originalKb);
    nodePorts.output<DecimalData>(OUTPUT_RATIO, originalKb / compressKb);
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()>
nitro::PngOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("PNG Compression", "Png", category);
        auto *valueLabel = new QLabel("-");
        auto *originalSizeLabel = new QLabel("-");
        auto *crLabel = new QLabel("-");
        return builder.
                withOperator(std::make_unique<nitro::PngOperator>(valueLabel, originalSizeLabel, crLabel))->
                withIcon("compress.png")->
                withDisplayWidget(DISPLAY_LABEL_ORIG, originalSizeLabel)->
                withDisplayWidget(DISPLAY_LABEL_COMP, valueLabel)->
                withDisplayWidget(DISPLAY_LABEL_RATIO, crLabel)->
                withNodeColor(NITRO_OUTPUT_COLOR)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_QUALITY, 9, 0, 9, BoundMode::UPPER_LOWER)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                withOutputValue(OUTPUT_ORIG_SIZE)->
                withOutputValue(OUTPUT_COMP_SIZE)->
                withOutputValue(OUTPUT_RATIO)->
                build();
    };
}

