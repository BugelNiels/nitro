#include "zliboperator.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include "nodes/datatypes/grayimagedata.hpp"

#define DISPLAY_LABEL_COMP "compLabel"
#define DISPLAY_LABEL_ORIG "origLabel"
#define DISPLAY_LABEL_RATIO "ratioLabel"
#define INPUT_IMAGE "Image"
#define OUTPUT_IMAGE "Compressed Image"
#define INPUT_BITS "Bits"

#define OUTPUT_COMP_SIZE "Compressed"
#define OUTPUT_ORIG_SIZE "Original"
#define OUTPUT_RATIO "Ratio"

#include <QDebug>

#include <zlib.h>
#include <opencv2/imgcodecs.hpp>

static void toIndexed(const cv::Mat &src, cv::Mat &dest, std::vector<float> &colTable) {

    std::vector<float> uniqueColors;
    std::set<float> uniqueColorsSet(src.begin<float>(), src.end<float>());
    colTable.assign(uniqueColorsSet.begin(), uniqueColorsSet.end());

    std::unordered_map<int, int> colMap;
    int numCols = colTable.size();
    for (int i = 0; i < numCols; i++) {
        colMap[colTable[i] * 255 + 0.5] = i;
    }

    int height = src.rows;
    int width = src.cols;
    dest = cv::Mat(height, width, CV_8UC1);
    for (int y = 0; y < height; y++) {
        uchar *rowPtr = dest.ptr<uchar>(y);
        const float *srcPtr = src.ptr<float>(y);
        for (int x = 0; x < width; x++) {
            rowPtr[x] = colMap[srcPtr[x] * 255 + 0.5];
        }
    }
}

static std::vector<uchar> packData(const cv::Mat &data, int numBits) {
    int rows = data.rows;
    int cols = data.cols;
    std::vector<uchar> packedData;
    packedData.reserve(rows * cols / (8.0 / numBits) + 0.5);
    uchar packedByte = 0;
    int bitPosition = 0;
    uchar leastBitMask = ((1 << numBits) - 1);

    for (int row = 0; row < rows; ++row) {
        const uchar *ptr = data.ptr<uchar>(row);

        for (int col = 0; col < cols; ++col) {
            uchar value = ptr[col];
            packedByte |= (value & leastBitMask) << bitPosition;
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

static std::vector<uchar> compressData(const std::vector<uchar> &data) {
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

static cv::Mat unpackData(const std::vector<uchar> &packedData, int numBits, int rows, int cols) {
    cv::Mat data(rows, cols, CV_8UC1);
    int bitPosition = 0;
    int packedIndex = 0;
    uchar leastBitMask = ((1 << numBits) - 1);

    for (int row = 0; row < rows; ++row) {
        uchar *ptr = data.ptr<uchar>(row);

        for (int col = 0; col < cols; ++col) {
            uchar value = (packedData[packedIndex] >> bitPosition) & leastBitMask;
            ptr[col] = value;


            bitPosition += numBits;
            if (bitPosition >= 8) {
                bitPosition -= 8;
                if (packedIndex < packedData.size()) {
                    packedIndex++;
                }
            }
        }
    }
    return data;
}

static std::vector<uchar> decompressData(const std::vector<uchar> &compressedData, uLong sourceSize) {
    std::vector<uchar> decompressedData(sourceSize);


    uLong destSize = static_cast<uLong>(decompressedData.size());
    int result = uncompress(&decompressedData[0], &destSize, &compressedData[0], compressedData.size());
    if (result != Z_OK) {
        // Decompression failed
        decompressedData.clear();
    }
    return decompressedData;
}

nitro::ZLibOperator::ZLibOperator(QLabel *valueLabel, QLabel *originalSizeLabel,
                                  QLabel *ratioLabel)
        : valueLabel_(valueLabel), originalSizeLabel_(originalSizeLabel), ratioLabel_(ratioLabel) {}

void nitro::ZLibOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto img = *nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    int bits = nodePorts.inputInteger(INPUT_BITS);


    cv::Mat data;
    std::vector<float> colTable;
    toIndexed(img, data, colTable);


    auto packedData = packData(data, bits);
    auto zlib_buffer = compressData(packedData);
    unsigned long size = zlib_buffer.size();

    auto decompressedData = decompressData(zlib_buffer, static_cast<uLong>(packedData.size()));
    cv::Mat unpackedIndexed = unpackData(packedData, bits, data.rows, data.cols);

    int height = unpackedIndexed.rows;
    int width = unpackedIndexed.cols;
    cv::Mat result(height, width, CV_32FC1);
    for (int y = 0; y < height; y++) {
        float *rowPtr = result.ptr<float>(y);
        const uchar *srcPtr = unpackedIndexed.ptr<uchar>(y);
        for (int x = 0; x < width; x++) {
            rowPtr[x] = colTable[srcPtr[x]];
        }
    }

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
nitro::ZLibOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("zlib  Compression", "zlib", category);
        auto *valueLabel = new QLabel("-");
        auto *originalSizeLabel = new QLabel("-");
        auto *crLabel = new QLabel("-");
        return builder.
                withOperator(std::make_unique<nitro::ZLibOperator>(valueLabel, originalSizeLabel, crLabel))->
                withIcon("compress.png")->
                withDisplayWidget(DISPLAY_LABEL_ORIG, originalSizeLabel)->
                withDisplayWidget(DISPLAY_LABEL_COMP, valueLabel)->
                withDisplayWidget(DISPLAY_LABEL_RATIO, crLabel)->
                withNodeColor(NITRO_OUTPUT_COLOR)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_BITS, 8, 1, 16, BoundMode::UPPER_LOWER)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                withOutputValue(OUTPUT_ORIG_SIZE)->
                withOutputValue(OUTPUT_COMP_SIZE)->
                withOutputValue(OUTPUT_RATIO)->
                build();
    };
}

