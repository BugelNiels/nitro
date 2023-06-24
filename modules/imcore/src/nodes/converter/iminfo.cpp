#include "iminfo.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include "nodes/datatypes/integerdata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define OUTPUT_WIDTH "Width"
#define OUTPUT_HEIGHT "Height"
#define OUTPUT_NUM_PIXELS "Num Pixels"
#define OUTPUT_TYPE "Type"

nitro::ImInfoOperator::ImInfoOperator(QLabel *typeLabel)
        : typeLabel_(typeLabel) {

}

void
nitro::ImInfoOperator::execute(NodePorts &nodePorts) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    auto im1 = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    nodePorts.output<IntegerData>(OUTPUT_WIDTH, im1->cols);
    nodePorts.output<IntegerData>(OUTPUT_HEIGHT, im1->rows);
    nodePorts.output<IntegerData>(OUTPUT_NUM_PIXELS, im1->cols * im1->rows);

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

    typeLabel_->setText(QString("Type: %1").arg(type));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ImInfoOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Image Info", "imInfo", category);
        auto *typeLabel = new QLabel("Type: ");
        return builder.
                withOperator(std::make_unique<nitro::ImInfoOperator>(typeLabel))->
                withIcon("info.png")->
                withNodeColor(NITRO_CONVERTER_COLOR)->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                withDisplayWidget(OUTPUT_TYPE, typeLabel)->
                withOutputInteger(OUTPUT_WIDTH)->
                withOutputInteger(OUTPUT_HEIGHT)->
                withOutputInteger(OUTPUT_NUM_PIXELS)->
                build();
    };
}
