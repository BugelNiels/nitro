#include "boxfilter.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

void nitro::BoxFilterOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {

    bool imPresent, sizePresent;
    auto inputImg = nodePorts.getInputImage("Image", imPresent);
    int kSize = nodePorts.getInputInteger("Size", sizePresent);
    if (!imPresent || !sizePresent) {
        return;
    }
    int option = options.at("Mode");
    cv::Mat result;
    switch (option) {
        case 0:
            cv::blur(*inputImg, result, cv::Size(kSize, kSize));
            break;
        case 1:
            kSize = kSize % 2 == 0 ? std::max(kSize - 1, 1) : kSize;
            cv::medianBlur(*inputImg, result, kSize);
            break;
        default:
            cv::blur(*inputImg, result, cv::Size(kSize, kSize));
            break;
    }
    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(result));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::BoxFilterOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Box Filter", "boxFilter", category);
        return builder.
                withOperator(std::make_unique<nitro::BoxFilterOperator>())->
                withIcon(":/icons/nodes/blur.png")->
                withNodeColor({71, 47, 189})->
                withDropDown("Mode", {"Average", "Median"})->
                withInputImage("Image")->
                withInputInteger("Size", 5, 1, 256)->
                withOutputImage("Image")->
                build();
    };
}
