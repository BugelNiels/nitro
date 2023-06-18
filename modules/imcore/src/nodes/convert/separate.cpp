#include "separate.hpp"
#include "nodes/nitronodebuilder.hpp"

#define INPUT_IMAGE "Image"

void nitro::SeparateOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    if (!nodePorts.inputsPresent({INPUT_IMAGE})) {
        return;
    }
    auto inputImg = nodePorts.getInputImage(INPUT_IMAGE);

    std::vector<cv::Mat> channels;
    if (inputImg->channels() == 1) {
        channels.resize(3);
        for (auto & channel : channels) {
            inputImg->copyTo(channel);
        }
    } else {
        cv::split(*inputImg, channels);
    }
    for (int i = 0; i < channels.size(); i++) {
        QString name = QString("Channel %1").arg(i + 1);
        nodePorts.setOutputImage(name, std::make_shared<cv::Mat>(channels[i]));
    }
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::SeparateOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Separate Channels", "separateChannels", category);
        return builder.
                withOperator(std::make_unique<nitro::SeparateOperator>())->
                withIcon("layers.png")->
                withNodeColor({36, 98, 131})->
                withInputImage("Image")->
                withOutputImage("Channel 1")->
                withOutputImage("Channel 2")->
                withOutputImage("Channel 3")->
                build();
    };
}
