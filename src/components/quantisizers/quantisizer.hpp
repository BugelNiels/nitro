#pragma once

#include "cbdimage.hpp"

namespace nitro {
    class Quantisizer {
    public:
        Quantisizer();

        virtual ~Quantisizer();

        CbdImage quantisize(const CbdImage &img, int newNumGreyLevels, bool dither = false) const;

    protected:
        CbdImage quantisizeDither(const CbdImage &img, int newNumGreyLevels) const;

        virtual QVector<int> constructColorTable(const CbdImage &img, int newNumGreyLevels) const = 0;

        virtual int findClosestValue(const CbdImage &img, const QVector<int> &newCols, int val) const = 0;

        virtual int findClosestValue(const CbdImage &img, const QVector<int> &newCols, float val) const = 0;
    };
}  // namespace nitro
