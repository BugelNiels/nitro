#include "combineoperator.hpp"
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
        if(channels[i].size != channels[0].size || channels[i].depth() != channels[0].depth()) {
            return;
        }
    }
    cv::Mat result;
    cv::merge(channels, result);
    nodePorts.setOutputImage("Image", std::make_shared<cv::Mat>(result));

}
