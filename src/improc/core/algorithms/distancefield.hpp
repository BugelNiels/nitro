#pragma once

#include <QImage>

#include "src/improc/core/matrix.hpp"
#include "src/improc/core/cbdimage.hpp"

namespace nitro {


    class DistanceField {
    public:
        DistanceField();

        explicit DistanceField(const CbdImage &img);

        bool valid() const;

        ~DistanceField() = default;

        const QImage &getDisplayImg(int index = 0);

        inline int width() const { return dfWidth; }

        inline int height() const { return dfHeight; }

        inline int numLevels() const { return numDfLevels; }

        inline const Matrix<float> &getDistField(int idx) const { return distanceField[idx]; }

    private:
        void calcDistanceField(const CbdImage &img);

        Matrix<float> calcDistanceFieldFast(const Matrix<int> &image, int d);

        int dfWidth;
        int dfHeight;
        int numDfLevels;

        QVector<Matrix<float>> distanceField;
        QVector<QImage> displayImages;
    };

}  // namespace nitro
