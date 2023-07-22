#include "rgbinput.hpp"
#include <nodes/nitronodebuilder.hpp>
#include "include/colimagedata.hpp"
#include <nodes/datatypes/decimaldata.hpp>

#include <opencv2/imgproc.hpp>
#include <random>

namespace nitro::ImCore {

static inline const QString INPUT_R = "Red";
static inline const QString INPUT_G = "Green";
static inline const QString INPUT_B = "Blue";
static inline const QString OUTPUT_COL = "Color";


RgbOperator::RgbOperator(QLabel *colLabel) : colLabel_(colLabel) {}

void RgbOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    double r = nodePorts.inputValue(INPUT_R);
    double g = nodePorts.inputValue(INPUT_G);
    double b = nodePorts.inputValue(INPUT_B);

    if (colLabelPixMap_.size() != colLabel_->size()) {
        colLabelPixMap_ = QPixmap(colLabel_->width(), colLabel_->height());
    }
    colLabelPixMap_.fill(QColor(r * 255, g * 255, b * 255));
    colLabel_->setPixmap(colLabelPixMap_);

    cv::Mat col(1, 1, CV_32FC3, cv::Scalar(r, g, b));
    nodePorts.output<ColImageData>(OUTPUT_COL, col);
}

std::function<std::unique_ptr<NitroNode>()> RgbOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("RGB", "rgb", category);
        QLabel *colLabel = new QLabel();
        return builder.
                withOperator(std::make_unique<RgbOperator>(colLabel))->
                withIcon("color.png")->
                withNodeColor(NITRO_INPUT_COLOR)->
                withDisplayWidget("Color", colLabel)->
                withInputValue(INPUT_R, .5, 0, 1, BoundMode::UPPER_LOWER)->
                withInputValue(INPUT_G, .5, 0, 1, BoundMode::UPPER_LOWER)->
                withInputValue(INPUT_B, .5, 0, 1, BoundMode::UPPER_LOWER)->
                withOutputPort<ColImageData>(OUTPUT_COL)->
                build();
    };
}

} // namespace nitro::ImCore
