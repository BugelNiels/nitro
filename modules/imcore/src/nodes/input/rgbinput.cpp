#include "rgbinput.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include "nodes/datatypes/decimaldata.hpp"
#include <opencv2/imgproc.hpp>
#include <random>

#define INPUT_R "Red"
#define INPUT_G "Green"
#define INPUT_B "Blue"
#define OUTPUT_COL "Color"


nitro::RgbOperator::RgbOperator(QLabel *colLabel) : colLabel_(colLabel) {}

void nitro::RgbOperator::execute(NodePorts &nodePorts) {
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

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::RgbOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("RGB", "rgb", category);
        QLabel *colLabel = new QLabel();
        return builder.
                withOperator(std::make_unique<nitro::RgbOperator>(colLabel))->
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

