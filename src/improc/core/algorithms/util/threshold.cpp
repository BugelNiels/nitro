#include "threshold.hpp"
#include "datamodels/datatypes/imagedata.hpp"
#include "datamodels/datatypes/integerdata.hpp"

nitro::CbdImage nitro::operations::threshold(const CbdImage &inputImg, int threshold, bool greater) {
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

void nitro::operations::ThresholdAlgorithm::compute(const std::map<QString, std::shared_ptr<QtNodes::NodeData>> &input,
                                                    std::map<QString, std::shared_ptr<QtNodes::NodeData>> &output,
                                                    const std::map<QString, QString> &options) const {

    auto inputImgDat = std::dynamic_pointer_cast<nitro::ImageData>(input.at("image"));
    auto thresholdDat = std::dynamic_pointer_cast<nitro::IntegerData>(input.at("threshold"));

    if(inputImgDat == nullptr || thresholdDat == nullptr) {
        return;
    }

    if(inputImgDat->isGrayscaleImg()) {
        auto result = threshold(*inputImgDat->image(), thresholdDat->value(), true);
        auto ptrRes = std::make_shared<nitro::CbdImage>(result);
        output["image"] = std::make_shared<nitro::ImageData>(ptrRes);
    }


}
