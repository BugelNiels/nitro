#pragma once

#include "quantisizer.hpp"

namespace nitro {

    class KMeansQuantisizer : public Quantisizer {
    public:
        KMeansQuantisizer();

        ~KMeansQuantisizer() override;

    protected:
        virtual QVector<int> constructColorTable(const CbdImage &img, int newNumGreyLevels) const override;

        virtual int findClosestValue(const CbdImage &img, const QVector<int> &newCols, int val) const override;

        virtual int findClosestValue(const CbdImage &img, const QVector<int> &newCols, float val) const override;
    };
}  // namespace nitro
