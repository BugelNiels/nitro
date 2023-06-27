#include "compressionsize.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include "nodes/datatypes/grayimagedata.hpp"

#define DISPLAY_LABEL_COMP "compLabel"
#define DISPLAY_LABEL_ORIG "origLabel"
#define DISPLAY_LABEL_RATIO "ratioLabel"
#define INPUT_IMAGE "Image"
#define INPUT_BITS "Bits"

#define OUTPUT_COMP_SIZE "Compressed"
#define OUTPUT_ORIG_SIZE "Original"
#define OUTPUT_RATIO "Ratio"

#include <QDebug>

#include <zlib.h>

std::vector<uchar> packData(cv::Mat &data, int numBits) {
    int rows = data.rows;
    int cols = data.cols;
    std::vector<uchar> packedData;
    packedData.reserve(rows * cols / (8.0 / numBits) + 0.5);
    uchar packedByte = 0;
    int bitPosition = 0;


    for (int row = 0; row < rows; ++row) {
        uchar *ptr = data.ptr<uchar>(row);

        for (int col = 0; col < cols; ++col) {
            uchar value = ptr[col];
            packedByte |= (value & ((1 << numBits) - 1)) << bitPosition;
            bitPosition += numBits;

            while (bitPosition >= 8) {
                packedData.push_back(packedByte);
                packedByte >>= 8;
                bitPosition -= 8;
            }
        }
    }

    if (bitPosition > 0) {
        packedData.push_back(packedByte);
    }

    return packedData;
}

std::vector<uchar> compressData(const std::vector<uchar> &data) {
    std::vector<uchar> compressedData;

    uLong sourceSize = static_cast<uLong>(data.size());
    uLong compressedSize = compressBound(sourceSize);
    compressedData.resize(compressedSize);

    int result = compress(&compressedData[0], &compressedSize, &data[0], sourceSize);
    if (result == Z_OK) {
        compressedData.resize(compressedSize);
    } else {
        // Compression failed
        compressedData.clear();
    }

    return compressedData;
}

nitro::CompressionSizeOperator::CompressionSizeOperator(QLabel *valueLabel, QLabel *originalSizeLabel,
                                                        QLabel *ratioLabel)
        : valueLabel_(valueLabel), originalSizeLabel_(originalSizeLabel), ratioLabel_(ratioLabel) {}

void nitro::CompressionSizeOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto img = *nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    int bits = nodePorts.inputInteger(INPUT_BITS);

    cv::Mat data;
    img.convertTo(data, CV_8U, 255);

    auto packedData = packData(img, bits);
    auto compressedData = compressData(packedData);
    unsigned long size = compressedData.size();

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
}

std::function<std::unique_ptr<nitro::NitroNode>()>
nitro::CompressionSizeOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Compression Size", "compressionSize", category);
        auto *valueLabel = new QLabel("-");
        auto *originalSizeLabel = new QLabel("-");
        auto *crLabel = new QLabel("-");
        return builder.
                withOperator(std::make_unique<nitro::CompressionSizeOperator>(valueLabel, originalSizeLabel, crLabel))->
                withIcon("number.png")->
                withDisplayWidget(DISPLAY_LABEL_ORIG, originalSizeLabel)->
                withDisplayWidget(DISPLAY_LABEL_COMP, valueLabel)->
                withDisplayWidget(DISPLAY_LABEL_RATIO, crLabel)->
                withNodeColor(NITRO_OUTPUT_COLOR)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_BITS, 8, 1, 16, BoundMode::UPPER_LOWER)->
                withOutputValue(OUTPUT_COMP_SIZE)->
                withOutputValue(OUTPUT_ORIG_SIZE)->
                withOutputValue(OUTPUT_RATIO)->
                build();
    };
}

