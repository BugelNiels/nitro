#include "combine.hpp"
#include "nodes/nitronodebuilder.hpp"
#include <opencv2/imgproc.hpp>

void nitro::CombineOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {

    std::vector<cv::Mat> channels;
    channels.resize(3);
    for (int i = 0; i < channels.size(); i++) {
        QString name = QString("Channel%1").arg(i + 1);
        bool imPresent;
        auto inputImg = nodePorts.getInputImage(name, imPresent);
        if (!imPresent) {
            return;
        }
        channels[i] = *inputImg;
    }
    for (int i = 1; i < channels.size(); i++) {
        if (channels[i].size != channels[0].size || channels[i].depth() != channels[0].depth()) {
            return;
        }
    }
    cv::Mat result;
    cv::merge(channels, result);
    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(result));

}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::CombineOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Combine Channels", "combineChannels", category);
        return builder.
                withOperator(std::make_unique<nitro::CombineOperator>())->
                withIcon(":/icons/nodes/combine.png")->
                withNodeColor({36, 98, 131})->
                withInputImage("Channel1")->
                withInputImage("Channel2")->
                withInputImage("Channel3")->
                withOutputImage("Image")->
                build();
    };
}
