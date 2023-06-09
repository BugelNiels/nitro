#include "threshold.hpp"
#include "nodes/datatypes/greyimagedata.hpp"
#include "nodes/datatypes/integerdata.hpp"

nitro::CbdImage threshold(const nitro::CbdImage &inputImg, int threshold, bool greater) {
    int width = inputImg.width();
    int height = inputImg.height();
    nitro::CbdImage result(width, height, 2);

    auto &inData = inputImg.constData();
    auto &outData = result.data();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int val = inData.get(x, y);
            bool satisfies = greater ? val >= threshold : val <= threshold;
            outData.set(x, y, satisfies ? 1 : 0);
        }
    }
    result.setIndexed({0, 255});
    return result;
}

void
nitro::ThresholdOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) const {

    auto inputImgDat = nodePorts.getInDataAsType<nitro::GreyImageData>("image");
    auto thresholdDat = nodePorts.getInDataAsType<nitro::IntegerData>("threshold");

    if (inputImgDat == nullptr || thresholdDat == nullptr) {
        return;
    }

    bool greater = options.at("Mode") == 1;
    auto result = threshold(*inputImgDat->image(), thresholdDat->value(), greater);
    auto ptrRes = std::make_shared<nitro::CbdImage>(result);
    nodePorts.setOutputData("image", std::make_shared<nitro::GreyImageData>(ptrRes));


}
