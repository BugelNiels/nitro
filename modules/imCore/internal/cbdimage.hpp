#pragma once

#include <QImage>

#include "matrix.hpp"

namespace nitro {

    class Resampler;

    class CbdImage {
    public:
        CbdImage();

        CbdImage(int width, int height, int dynRange);

        explicit CbdImage(const QImage &img);

        const QImage &getDisplayImg();

        [[nodiscard]] inline int width() const { return matrix.width(); }

        [[nodiscard]] inline int height() const { return matrix.height(); }

        [[nodiscard]] inline int numLevels() const { return numGreyLevels; }

        [[nodiscard]] int get(int x, int y) const;

        inline Matrix<int> &data() { return matrix; }

        [[nodiscard]] inline const Matrix<int> &constData() const { return matrix; }

        [[nodiscard]] inline const QVector<int> &getColTransform() const { return vals; }

        [[nodiscard]] inline bool isIndexed() const { return indexed; }


        // pass by value on purpose
        void setIndexed(QVector<int> vals);

    private:
        int numGreyLevels = 0;
        Matrix<int> matrix;

        QImage displayImg;
        bool displayCalculated = false;

        bool indexed = false;
        QVector<int> vals;

    };

}  // namespace nitro
