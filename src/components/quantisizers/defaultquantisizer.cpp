#include "defaultquantisizer.hpp"

nitro::DefaultQuantisizer::DefaultQuantisizer() = default;

nitro::DefaultQuantisizer::~DefaultQuantisizer() = default;

QVector<int> nitro::DefaultQuantisizer::constructColorTable(
        const CbdImage &img, int newNumGreyLevels) const {
    QVector<int> newCols;
    newCols.resize(newNumGreyLevels);

    for (int i = 0; i < newNumGreyLevels; i++) {
        newCols[i] = i * 255 / float(newNumGreyLevels - 1);
    }
    return newCols;
}

int nitro::DefaultQuantisizer::findClosestValue(const CbdImage &img,
                                                const QVector<int> &newCols,
                                                int val) const {
    int dynRange = img.numLevels();
    int newNumGreyLevels = newCols.size();
    float proportion = val / float(dynRange);
    int newVal = proportion * (newNumGreyLevels - 1) + 0.5f;
    return newVal;
}

int nitro::DefaultQuantisizer::findClosestValue(const CbdImage &img,
                                                const QVector<int> &newCols,
                                                float val) const {
    int dynRange = img.numLevels();
    int newNumGreyLevels = newCols.size();
    float proportion = val / float(dynRange);
    int newVal = proportion * (newNumGreyLevels - 1) + 0.5f;
    return newVal;
}
