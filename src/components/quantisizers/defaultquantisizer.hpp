#pragma once

#include "quantisizer.hpp"

namespace nitro {

    class DefaultQuantisizer : public Quantisizer {
    public:
        DefaultQuantisizer();

        ~DefaultQuantisizer() override;

    protected:
        QVector<int> constructColorTable(const CbdImage &img, int newNumGreyLevels) const override;

        int findClosestValue(const CbdImage &img, const QVector<int> &newCols, int val) const override;

        int findClosestValue(const CbdImage &img, const QVector<int> &newCols, float val) const override;
    };

}  // namespace nitro
