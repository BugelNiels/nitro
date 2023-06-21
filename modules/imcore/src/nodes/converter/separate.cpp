#include "separate.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"

#define INPUT_IMAGE "Image"

void nitro::SeparateOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    auto inputImg = nodePorts.inGet<ImageData>(INPUT_IMAGE).data();

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
        nodePorts.output<ImageData>(name, channels[i]);
    }
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::SeparateOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Separate Channels", "separateChannels", category);
        return builder.
                withOperator(std::make_unique<nitro::SeparateOperator>())->
                withIcon("layers.png")->
                withNodeColor(NITRO_CONVERTER_COLOR)->
                withInputPort<ImageData>("Image")->
                withOutputPort<ImageData>("Channel 1")->
                withOutputPort<ImageData>("Channel 2")->
                withOutputPort<ImageData>("Channel 3")->
                build();
    };
}
