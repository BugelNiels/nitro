#include "bilateralfilter.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"
#include <opencv2/imgproc.hpp>

#define INPUT_IMAGE "Image"
#define INPUT_SIGMA_C "Sigma (c)"
#define INPUT_SIGMA_S "Sigma (s)"
#define INPUT_D "Diameter"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"

void nitro::BilateralFilterOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    // Get the input data
    auto inputImg = nodePorts.inGet<ImageData>(INPUT_IMAGE).data();
    double sigmaCol = nodePorts.inputValue(INPUT_SIGMA_C);
    double sigmaSpace = nodePorts.inputValue(INPUT_SIGMA_S);
    int d = nodePorts.inputInteger(INPUT_D);

    // Perform filtering
    cv::Mat result;
    cv::bilateralFilter(*inputImg, result, d, sigmaCol, sigmaSpace);

    // Store the result
    nodePorts.output<ImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::BilateralFilterOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Bilateral Filter", "bilateralFilter", category);
        return builder.
                withOperator(std::make_unique<nitro::BilateralFilterOperator>())->
                withIcon("blur.png")->
                withNodeColor({71, 47, 189})->
                withInputPort<ImageData>(INPUT_IMAGE)->
                withInputInteger(INPUT_D, 9, 1, 64)->
                withInputValue(INPUT_SIGMA_C, 75, 2, 255)->
                withInputValue(INPUT_SIGMA_S, 75, 2, 255)->
                withOutputPort<ImageData>(OUTPUT_IMAGE)->
                build();
    };
}
