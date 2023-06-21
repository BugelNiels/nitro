#include "combine.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include <opencv2/imgproc.hpp>

#define OUTPUT_IMAGE "Image"

void
nitro::CombineOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }

    std::vector<cv::Mat> channels;
    channels.resize(3);
    for (int i = 0; i < channels.size(); i++) {
        QString name = QString("Channel %1").arg(i + 1);
        auto inputImg = GrayImageData::from(nodePorts.inGet(name));
        channels[i] = *inputImg;
    }
    for (int i = 1; i < channels.size(); i++) {
        if (channels[i].size != channels[0].size || channels[i].depth() != channels[0].depth()) {
            return;
        }
    }
    cv::Mat result;
    cv::merge(channels, result);
    nodePorts.output<ColImageData>(OUTPUT_IMAGE, result);

}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::CombineOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Combine Channels", "combineChannels", category);
        return builder.
                withOperator(std::make_unique<nitro::CombineOperator>())->
                withIcon("layers.png")->
                withNodeColor(NITRO_CONVERTER_COLOR)->
                withInputPort<GrayImageData>("Channel 1")->
                withInputPort<GrayImageData>("Channel 2")->
                withInputPort<GrayImageData>("Channel 3")->
                withOutputPort<ColImageData>(OUTPUT_IMAGE)->
                build();
    };
}
