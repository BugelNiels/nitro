#pragma once

#include <QImage>

#include "util/matrix.hpp"

namespace nitro {

    class Resampler;

    class CbdImage {
    public:
        CbdImage();

        CbdImage(const CbdImage &img);

        CbdImage &operator=(const CbdImage &img) = default;

        CbdImage(int width, int height, int dynRange);

        explicit CbdImage(const QImage &img);

        const QImage &getDisplayImg();

        inline int width() const { return matrix.width(); }

        inline int height() const { return matrix.height(); }

        inline int numLevels() const { return numGreyLevels; }

        inline Matrix<int> &data() { return matrix; }

        inline const Matrix<int> &constData() const { return matrix; }

        inline const QVector<int> &getColTransform() const { return vals; }

        inline bool isIndexed() const { return indexed; }

        // pass by value on purpose
        void setIndexed(QVector<int> vals);

    private:
        int numGreyLevels;
        Matrix<int> matrix;

        QImage displayImg;
        bool displayCalculated = false;

        bool indexed = false;
        QVector<int> vals;
    };

}  // namespace nitro
