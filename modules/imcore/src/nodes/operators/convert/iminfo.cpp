#include "iminfo.hpp"
#include "util.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define OUTPUT_WIDTH "Width"
#define OUTPUT_HEIGHT "Height"

nitro::ImInfoOperator::ImInfoOperator(QLabel *widthLabel, QLabel *heightLabel)
        : widthLabel_(widthLabel),
          heightLabel_(heightLabel) {

}

void nitro::ImInfoOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {
    if (!nodePorts.inputsPresent({INPUT_IMAGE})) {
        widthLabel_->setText("Width: -");
        heightLabel_->setText("Height: -");
        return;
    }
    auto im1 = nodePorts.getInputImage(INPUT_IMAGE);
    nodePorts.setOutputInteger(OUTPUT_WIDTH, im1->cols);
    nodePorts.setOutputInteger(OUTPUT_HEIGHT, im1->rows);

    widthLabel_->setText(QString("Width: %1").arg(im1->cols));
    heightLabel_->setText(QString("Height: %1").arg(im1->rows));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ImInfoOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Image Info", "imInfo", category);
        auto *widthLabel = new QLabel("Width: ");
        auto *heightLabel = new QLabel("Height: ");
        return builder.
                withOperator(std::make_unique<nitro::ImInfoOperator>(widthLabel, heightLabel))->
                withIcon("info.png")->
                withNodeColor({36, 98, 131})->
                withInputImage(INPUT_IMAGE)->
                withDisplayWidget(OUTPUT_WIDTH, widthLabel)->
                withDisplayWidget(OUTPUT_HEIGHT, heightLabel)->
                withOutputInteger(OUTPUT_WIDTH)->
                withOutputInteger(OUTPUT_HEIGHT)->
                build();
    };
}
