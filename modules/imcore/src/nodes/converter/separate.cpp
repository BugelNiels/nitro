#include "separate.hpp"
#include "include/colimagedata.hpp"
#include "include/grayimagedata.hpp"
#include <nodes/nitronodebuilder.hpp>

namespace nitro::ImCore {

static inline const QString INPUT_IMAGE = "Image";

void SeparateOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto inputImg = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);

    std::vector<cv::Mat> channels;
    if (inputImg->channels() == 1) {
        channels.resize(3);
        for (auto &channel: channels) {
            inputImg->copyTo(channel);
        }
    } else {
        cv::split(*inputImg, channels);
    }
    for (int i = 0; i < channels.size(); i++) {
        QString name = QString("Channel %1").arg(i + 1);
        nodePorts.output<GrayImageData>(name, channels[i]);
    }
}

std::function<std::unique_ptr<NitroNode>()> SeparateOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Separate Channels", "separateChannels", category);
        return builder.withOperator(std::make_unique<SeparateOperator>())
                ->withIcon("layers.png")
                ->withNodeColor(NITRO_CONVERTER_COLOR)
                ->withInputPort<ColImageData>("Image")
                ->withOutputPort<GrayImageData>("Channel 1")
                ->withOutputPort<GrayImageData>("Channel 2")
                ->withOutputPort<GrayImageData>("Channel 3")
                ->build();
    };
}

} // namespace nitro::ImCore
