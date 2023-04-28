#include "thresholddatamodel.hpp"

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>


nitro::ThresholdDataModel::ThresholdDataModel() = default;

nitro::CbdImage nitro::ThresholdDataModel::compute(const nitro::CbdImage &inputImg) {

    int width = inputImg.width();
    int height = inputImg.height();
    nitro::CbdImage result(width, height, 2);

    auto& inData = inputImg.constData();
    auto& outData = result.data();
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            int val = inData.get(x , y);
            outData.set(x, y, val <= threshold ? 0 : 1);
        }
    }
    result.setIndexed({0, 255});
    return result;
}

