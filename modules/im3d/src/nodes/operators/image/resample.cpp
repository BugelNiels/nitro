#include "resample.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

#include <QDebug>

void nitro::ResampleOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {
    bool kPresent, imPresent;
    int k = nodePorts.getInputInteger("K", kPresent);
    auto inputImg = nodePorts.getInputImage("Image", imPresent);

    if (!kPresent || !imPresent) {
        return;
    }


//    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(kMeansDat));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ResampleOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Resample", "resample", category);
        return builder.
                withOperator(std::make_unique<nitro::ResampleOperator>())->
                withIcon(":/icons/nodes/resample.png")->
                withNodeColor({201, 94, 6})->
                withDropDown("Mode", {"Linear", "Cubic"})->
                withInputImage("Image")->
                withInputInteger("Levels", 256, 1, 256)->
                withOutputImage("Image")->
                build();
    };
}
