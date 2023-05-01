#include "flipdatamodel.hpp"

#include <QtWidgets/QLineEdit>
#include <QImageReader>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>

#include <FLIP.h>

nitro::FlipDataModel::FlipDataModel() = default;


unsigned int nitro::FlipDataModel::nPorts(QtNodes::PortType portType) const {
    unsigned int result = 1;

    switch (portType) {
        case QtNodes::PortType::In:
            result = 2;
            break;

        case QtNodes::PortType::Out:
            result = 1;

        default:
            break;
    }

    return result;
}

QtNodes::NodeDataType nitro::FlipDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    return ImageData().type();
}


void nitro::FlipDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) {
    auto inputImg = std::dynamic_pointer_cast<ImageData>(data);

    if (!data || inputImg == nullptr) {
        Q_EMIT dataInvalidated(0);
        return;
    }
    // TODO: check that dimensions agree and otherwise invalidate and emit warning
    if (portIndex == 0) {
        _inputImgA = inputImg;
    }
    if (portIndex == 1) {
        _inputImgB = inputImg;
    }
    setFlipResult();
    Q_EMIT dataUpdated(0);
}


static FLIP::image<FLIP::color3> qColImgToFlipImg(const QImage &img) {
    int width = img.width();
    int height = img.height();

    FLIP::image<FLIP::color3> res(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QColor col = img.pixelColor(x, y);
            float r = col.red() / 255.0f;
            float g = col.green() / 255.0f;
            float b = col.blue() / 255.0f;
            res.set(x, y, FLIP::color3(r, g, b));
        }
    }
    return res;
}

static FLIP::image<FLIP::color3> qImgToFlipImg(const QImage &img) {
    int width = img.width();
    int height = img.height();

    FLIP::image<FLIP::color3> res(width, height);
    for (int y = 0; y < height; y++) {
        const uchar *row = img.constScanLine(y);
        for (int x = 0; x < width; x++) {
            res.set(x, y, FLIP::color3(row[x] / 255.0f));
        }
    }
    return res;
}

static QImage flipImgToQimg(const FLIP::image<FLIP::color3> &img) {
    int width = img.getWidth();
    int height = img.getHeight();

    QImage res(width, height, QImage::Format_RGB32);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            FLIP::color3 col = img.get(x, y);
            int r = 255 * col.r + 0.5f;
            int g = 255 * col.g + 0.5f;
            int b = 255 * col.b + 0.5f;
            res.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return res;
}

inline static float calculatePPD(const float dist, const float resolutionX,
                                 const float monitorWidth) {
    return dist * (resolutionX / monitorWidth) * (float(FLIP::PI) / 180.0f);
}

void nitro::FlipDataModel::setFlipResult() {
    if (_inputImgA == nullptr || _inputImgB == nullptr || !_inputImgA->isValid() || !_inputImgB->isValid()) {
        return;
    }
    const QImage &inA = _inputImgA->getDisplayImg();
    const QImage &inB = _inputImgB->getDisplayImg();
    FLIP::image<FLIP::color3> fImgA = _inputImgA->isColImg() ? qColImgToFlipImg(inA) : qImgToFlipImg(inA);
    FLIP::image<FLIP::color3> fImgB = _inputImgB->isColImg() ? qColImgToFlipImg(inB) : qImgToFlipImg(inB);


    int width = inA.width();
    int height = inA.height();
    int ppd = calculatePPD(0.7f, 1920, 0.7f);
    FLIP::image<float> errMap(width, height, 0.0f);
    errMap.FLIP(fImgA, fImgB, ppd);


    FLIP::image<FLIP::color3> colResult(width, height);
    colResult.copyFloat2Color3(errMap);
    FLIP::image<FLIP::color3> magmaMap(FLIP::MapMagma, 256);
    colResult.colorMap(errMap, magmaMap);
    auto finalRes = flipImgToQimg(colResult);
    updateImage(finalRes);
    // TODO: extract this somewhere?
    auto resPtr = std::make_shared<QImage>(finalRes);
    _result = std::make_shared<nitro::ImageData>(resPtr);
}


std::shared_ptr<QtNodes::NodeData> nitro::FlipDataModel::outData(QtNodes::PortIndex port) {
    return _result;
}

