#include "bilateralfilter.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_SIGMA_C "Sigma (c)"
#define INPUT_SIGMA_S "Sigma (s)"
#define INPUT_D "Diameter"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

void nitro::BilateralFilterOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    if (!nodePorts.inputsPresent({INPUT_IMAGE, INPUT_SIGMA_C, INPUT_SIGMA_S, INPUT_D})) {
        return;
    }
    auto inputImg = nodePorts.getInputImage(INPUT_IMAGE);
    double sigmaCol = nodePorts.getInputValue(INPUT_SIGMA_C);
    double sigmaSpace = nodePorts.getInputValue(INPUT_SIGMA_S);
    int d = nodePorts.getInputInteger(INPUT_D);
    cv::Mat result;
    cv::bilateralFilter(*inputImg, result, d, sigmaCol, sigmaSpace);
    nodePorts.setOutputImage(OUTPUT_IMAGE, std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::BilateralFilterOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Bilateral Filter", "bilateralFilter", category);
        return builder.
                withOperator(std::make_unique<nitro::BilateralFilterOperator>())->
                withIcon("blur.png")->
                withNodeColor({71, 47, 189})->
                withInputImage(INPUT_IMAGE)->
                withInputInteger(INPUT_D, 9, 1, 64)->
                withInputValue(INPUT_SIGMA_C, 75, 2, 255)->
                withInputValue(INPUT_SIGMA_S, 75, 2, 255)->
                withOutputImage(OUTPUT_IMAGE)->
                build();
    };
}
