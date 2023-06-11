#include "separateoperator.hpp"

void nitro::SeparateOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {
    bool imPresent;
    auto inputImg = nodePorts.getInputImage("Image", imPresent);
    if(!imPresent) {
        return;
    }

    std::vector<cv::Mat> channels;
    if(inputImg->channels() == 1) {
        channels.resize(3);
        for(int i = 0; i < channels.size(); i++) {
            inputImg->copyTo(channels[i]);
        }
    } else {
        cv::split(*inputImg, channels);
    }
    for(int i = 0; i < channels.size(); i++) {
        QString name = QString("Channel%1").arg(i + 1);
        nodePorts.setOutputImage(name, std::make_shared<cv::Mat>(channels[i]));
    }
}
