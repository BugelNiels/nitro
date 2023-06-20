#include "iminfo.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include "nodes/datatypes/integerdata.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define OUTPUT_WIDTH "Width"
#define OUTPUT_HEIGHT "Height"
#define OUTPUT_CHANNELS "Channels"
#define OUTPUT_TYPE "Type"
#define OUTPUT_MIN "Min"
#define OUTPUT_MAX "Max"

nitro::ImInfoOperator::ImInfoOperator(QLabel *typeLabel)
        : typeLabel_(typeLabel) {

}

void
nitro::ImInfoOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    auto im1 = nodePorts.inGet<ImageData>(INPUT_IMAGE).data();
    nodePorts.output<IntegerData>(OUTPUT_WIDTH, im1->cols);
    nodePorts.output<IntegerData>(OUTPUT_HEIGHT, im1->rows);
    nodePorts.output<IntegerData>(OUTPUT_CHANNELS, im1->channels());

    QString type;
    switch (im1->depth()) {
        case CV_8U:  // 8-bit unsigned integer (0-255)
            type = "8-bit uint";
            break;
        case CV_8S:  // 8-bit signed integer (-128 to 127)
            type = "8-bit int";
            break;
        case CV_16U:  // 16-bit unsigned integer (0-65535)
            type = "16-bit uint";
            break;
        case CV_16S:  // 16-bit signed integer (-32768 to 32767)
            type = "16-bit int";
            break;
        case CV_32S:  // 32-bit signed integer
            type = "32-bit int";
            break;

        case CV_32F:  // 32-bit floating-point
            type = "Float";
            break;
        case CV_64F:  // 64-bit floating-point
            type = "Double";
            break;
        default:
            type = "-";
            break;
    }


    double minValue, maxValue;
    std::vector<cv::Mat> channels;
    cv::split(*im1, channels);
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(channels[0], &minValue, &maxValue, &minLoc, &maxLoc);
    for (int i = 1; i < channels.size(); i++) {
        double minValueTemp, maxValueTemp;
        cv::minMaxLoc(channels[i], &minValueTemp, &maxValueTemp, &minLoc, &maxLoc);
        minValue = std::min(minValue, minValueTemp);
        maxValue = std::max(maxValue, maxValueTemp);
    }

    typeLabel_->setText(QString("Type: %1").arg(type));
    nodePorts.output<DecimalData>(OUTPUT_MIN, minValue);
    nodePorts.output<DecimalData>(OUTPUT_MAX, maxValue);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ImInfoOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Image Info", "imInfo", category);
        auto *typeLabel = new QLabel("Type: ");
        return builder.
                withOperator(std::make_unique<nitro::ImInfoOperator>(typeLabel))->
                withIcon("info.png")->
                withNodeColor({36, 98, 131})->
                withInputPort<ImageData>(INPUT_IMAGE)->
                withDisplayWidget(OUTPUT_TYPE, typeLabel)->
                withOutputInteger(OUTPUT_WIDTH)->
                withOutputInteger(OUTPUT_HEIGHT)->
                withOutputInteger(OUTPUT_CHANNELS)->
                withOutputValue(OUTPUT_MIN)->
                withOutputValue(OUTPUT_MAX)->
                build();
    };
}
