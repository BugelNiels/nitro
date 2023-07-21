#include "jpegoperator.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include "../../../../imcore/include/grayimagedata.hpp"

#include <opencv2/imgcodecs.hpp>

namespace nitro::Compression {

inline const QString DISPLAY_LABEL_COMP = "compLabel";
inline const QString DISPLAY_LABEL_ORIG = "origLabel";
inline const QString DISPLAY_LABEL_RATIO = "ratioLabel";
inline const QString INPUT_IMAGE = "Image";
inline const QString OUTPUT_IMAGE = "Compressed Image";
inline const QString INPUT_QUALITY = "Quality";
inline const QString OUTPUT_COMP_SIZE = "Compressed";
inline const QString OUTPUT_ORIG_SIZE = "Original";
inline const QString OUTPUT_RATIO = "Ratio";

JpegOperator::JpegOperator(QLabel *valueLabel, QLabel *originalSizeLabel,
                           QLabel *ratioLabel)
        : valueLabel_(valueLabel), originalSizeLabel_(originalSizeLabel), ratioLabel_(ratioLabel) {}

void JpegOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto img = *nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    int jpeg_quality = nodePorts.inputInteger(INPUT_QUALITY);

    cv::Mat data;
    img.convertTo(data, CV_8U, 255);


    std::vector<uchar> jpeg_buffer;
    std::vector<int> jpeg_params = {cv::IMWRITE_JPEG_QUALITY, jpeg_quality};
    cv::imencode(".jpg", data, jpeg_buffer, jpeg_params);
    unsigned long size = jpeg_buffer.size();

    cv::Mat result = cv::imdecode(jpeg_buffer, cv::IMREAD_GRAYSCALE);
    result.convertTo(result, CV_32F, 1.0 / 255.0);

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

std::function<std::unique_ptr<NitroNode>()> JpegOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("JPEG  Compression", "jpeg", category);
        auto *valueLabel = new QLabel("-");
        auto *originalSizeLabel = new QLabel("-");
        auto *crLabel = new QLabel("-");
        return builder.
                withOperator(std::make_unique<JpegOperator>(valueLabel, originalSizeLabel, crLabel))->
                withIcon("zip.png")->
                withDisplayWidget(DISPLAY_LABEL_ORIG, originalSizeLabel)->
                withDisplayWidget(DISPLAY_LABEL_COMP, valueLabel)->
                withDisplayWidget(DISPLAY_LABEL_RATIO, crLabel)->
                withNodeColor(NITRO_OUTPUT_COLOR)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_QUALITY, 90, 0, 100, BoundMode::UPPER_LOWER)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                withOutputValue(OUTPUT_ORIG_SIZE)->
                withOutputValue(OUTPUT_COMP_SIZE)->
                withOutputValue(OUTPUT_RATIO)->
                build();
    };
}

} // namespace nitro::Compression
