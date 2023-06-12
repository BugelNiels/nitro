#include "bilateralfilter.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

void nitro::BilateralFilterOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {

    bool imPresent, sigmaColPresent, sigmaSpacePresent, dPresent;
    auto inputImg = nodePorts.getInputImage("Image", imPresent);
    double sigmaCol = nodePorts.getInputValue("Sigma (c)", sigmaColPresent);
    double sigmaSpace = nodePorts.getInputValue("Sigma (s)", sigmaSpacePresent);
    int d = nodePorts.getInputInteger("d", dPresent);
    if (!imPresent || !sigmaColPresent || !sigmaSpacePresent || !dPresent) {
        return;
    }
    cv::Mat result;
    cv::bilateralFilter(*inputImg, result, d, sigmaCol, sigmaSpace);
    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::BilateralFilterOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Bilateral Filter", "bilateralFilter", category);
        return builder.
                withOperator(std::make_unique<nitro::BilateralFilterOperator>())->
                withIcon(":/icons/nodes/blur.png")->
                withNodeColor({71, 47, 189})->
                withInputImage("Image")->
                withInputInteger("d", 9, 1, 64)->
                withInputValue("Sigma (c)", 75, 2, 255)->
                withInputValue("Sigma (s)", 75, 2, 255)->
                withOutputImage("Image")->
                build();
    };
}
