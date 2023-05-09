#include <QPainter>
#include "imgmath.hpp"

#include "util/util.hpp"

nitro::CbdImage nitro::operations::addImage(const nitro::CbdImage &inputImg, int addVal) {
    int width = inputImg.width();
    int height = inputImg.height();
    nitro::CbdImage result(width, height, inputImg.numLevels());
    auto &inData = inputImg.constData();
    auto &outData = result.data();
    if (inputImg.isIndexed()) {
        QVector<int> outCols(inputImg.numLevels());
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                outData.set(x, y, inData.get(x, y));
            }
        }
        const auto &inCols = inputImg.getColTransform();
        for (int i = 0; i < inputImg.numLevels(); i++) {
            outCols[i] = nitro::clamp(inCols[i] + addVal);
        }
        result.setIndexed(outCols);
    } else {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                outData.set(x, y, nitro::clamp(inputImg.get(x, y) + addVal));
            }
        }
    }
    return result;
}

nitro::CbdImage nitro::operations::subtractImage(const nitro::CbdImage &inputImg, int subVal) {
    int width = inputImg.width();
    int height = inputImg.height();
    nitro::CbdImage result(width, height, inputImg.numLevels());
    auto &inData = inputImg.constData();
    auto &outData = result.data();
    if (inputImg.isIndexed()) {
        QVector<int> outCols(inputImg.numLevels());
        const auto &inCols = inputImg.getColTransform();
        for (int i = 0; i < inputImg.numLevels(); i++) {
            outCols[i] = nitro::clamp(inCols[i] - subVal);
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    outData.set(x, y, inData.get(x, y));
                }
            }
        }
        result.setIndexed(outCols);
    } else {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                outData.set(x, y, nitro::clamp(inData.get(x, y) - subVal));
            }
        }
    }
    return result;
}

nitro::CbdImage nitro::operations::subtractImage(const nitro::CbdImage &inputImg, const nitro::CbdImage &other) {
    int width = inputImg.width();
    int height = inputImg.height();
    nitro::CbdImage result(width, height, 256);
    auto &outData = result.data();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            outData.set(x, y, nitro::clamp(inputImg.get(x, y) - other.get(x, y)));
        }
    }

    return result;
}


nitro::CbdImage nitro::operations::multiplyImage(const nitro::CbdImage &inputImg, double mul) {
    int width = inputImg.width();
    int height = inputImg.height();
    nitro::CbdImage result(width, height, inputImg.numLevels());
    auto &inData = inputImg.constData();
    auto &outData = result.data();
    if (inputImg.isIndexed()) {
        QVector<int> outCols(inputImg.numLevels());
        const auto &inCols = inputImg.getColTransform();
        for (int i = 0; i < inputImg.numLevels(); i++) {
            outCols[i] = nitro::clampConvert(inCols[i] * mul);
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    outData.set(x, y, inData.get(x, y));
                }
            }
        }
        result.setIndexed(outCols);
    } else {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                outData.set(x, y, nitro::clampConvert(inData.get(x, y) * mul));
            }
        }
    }
    return result;
}


nitro::CbdImage nitro::operations::divideImage(const nitro::CbdImage &inputImg, double divVal) {
    int width = inputImg.width();
    int height = inputImg.height();
    nitro::CbdImage result(width, height, inputImg.numLevels());
    auto &inData = inputImg.constData();
    auto &outData = result.data();
    if (inputImg.isIndexed()) {
        QVector<int> outCols(inputImg.numLevels());
        const auto &inCols = inputImg.getColTransform();
        for (int i = 0; i < inputImg.numLevels(); i++) {
            outCols[i] = nitro::clampConvert(inCols[i] / divVal);
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    outData.set(x, y, inData.get(x, y));
                }
            }
        }
        result.setIndexed(outCols);
    } else {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                outData.set(x, y, nitro::clampConvert(inData.get(x, y) / divVal));
            }
        }
    }
    return result;
}

QImage nitro::operations::mixImage(const QImage &imgA, const QImage &imgB, float factor) {

    QImage result(imgA.width(), imgA.height(), QImage::Format_ARGB32);
    QPainter painter(&result);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(imgA.rect(), Qt::transparent);
    painter.setOpacity(1.0 - factor);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, imgA);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setOpacity(factor);
    painter.drawImage(0, 0, imgB);
    painter.end();
    return result.convertToFormat(QImage::Format_RGB32);
}

QImage nitro::operations::mixImage(const QImage &imgA, const QImage &imgB, QPainter::CompositionMode mode) {

    QImage result(imgA.width(), imgA.height(), QImage::Format_RGB32);
    QPainter painter(&result);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(imgA.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, imgA);
    painter.setCompositionMode(mode);
    painter.drawImage(0, 0, imgB);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    painter.fillRect(imgA.rect(), Qt::white);
    painter.end();
    return result;
}