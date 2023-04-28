#pragma once

#include <QImage>

#include "util/matrix.hpp"
#include "util/progressupdater.hpp"

namespace nitro {

    class Resampler;

    class CbdImage {
    public:
        CbdImage();

        CbdImage(const CbdImage &img);

        CbdImage(int width, int height, int dynRange);

        explicit CbdImage(const QString& string);

        explicit CbdImage(QImage &img);

        void calcDistanceField(ProgressUpdater *updater = nullptr);

        CbdImage resample(int numLevels, const std::shared_ptr<Resampler> &resampler);

        CbdImage compress(int targetBitDepth, const std::shared_ptr<Resampler> &resampler, ProgressUpdater *updater);

        CbdImage decompress(int targetBitDepth, const std::shared_ptr<Resampler> &resampler);

        QImage getDisplayImg();

        inline int width() const { return matrix.width(); }

        inline int height() const { return matrix.height(); }

        inline int numLevels() const { return numGreyLevels; }

        inline Matrix<int> &data() { return matrix; }

        inline const Matrix<int> &constData() const { return matrix; }

        // TODO: distance field class
        inline QVector<Matrix<float>> &getDistField() { return distanceField; }

        inline QVector<int> &getColTransform() { return vals; }

        inline bool isIndexed() const { return indexed; }

        // pass by value on purpose
        void setIndexed(QVector<int> vals);

    private:
        int diff(CbdImage &other);

        int numGreyLevels;
        Matrix<int> matrix;
        QVector<Matrix<float>> distanceField;

        QImage displayImg;
        bool displayCalculated = false;

        bool indexed = false;
        QVector<int> vals;
    };

}  // namespace nitro
