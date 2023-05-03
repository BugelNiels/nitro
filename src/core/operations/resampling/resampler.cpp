#include "resampler.hpp"

nitro::Resampler::Resampler() = default;

nitro::Resampler::~Resampler() = default;

nitro::CbdImage nitro::Resampler::resample(const CbdImage &image, const DistanceField &df, int numDesiredLevels) {
    int width = image.width();
    int height = image.height();

    CbdImage resampled(width, height, numDesiredLevels);
    auto &resampledData = resampled.data();
    //  const auto &inputData = image.constData();
    //  const auto &colTable = image.getColTransform();

    int numLevelsInput = image.numLevels();
#pragma omp parallel for default(none) firstprivate(height, width, numDesiredLevels, numLevelsInput) shared(image, df, resampledData)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // grey level of input pixel
            //      int dUpper, dLower;
            //      int val = inputData.get(x, y);
            //      if (image.isIndexed()) {
            //        int valNext = val + 2;
            //        int colNext = valNext >= numLevelsInput ? 255.0f :
            //        colTable[valNext]; dUpper = float(colNext) / 255.0f *
            //        (numDesiredLevels - 1); dLower = colTable[val] / (255.0) *
            //        (numDesiredLevels - 1);
            //      } else {
            //        dUpper =
            //            (val + 1) / (image.numLevels() - 1.0f) * (numDesiredLevels -
            //            1);
            //        dUpper = MIN(dUpper, numDesiredLevels - 1);
            //        dLower = val / (image.numLevels() - 1.0f) * (numDesiredLevels -
            //        1);
            //      }

            for (int d = numDesiredLevels - 1; d >= 0; d--) {
                //      for (int d = dUpper; d >= dLower; d--) {
                // how far along all grey levels we are
                float p = d / (numDesiredLevels - 1.0f);
                float dist;
                if (image.isIndexed()) {
                    dist = distFuncIndexed(image, df, x, y, p, numLevelsInput);
                } else {
                    dist = distFunc(image, df, x, y, p, numLevelsInput);
                }
                if (dist <= 0) {
                    resampledData.set(x, y, d);
                    break;
                }
            }
        }
    }
    if (numDesiredLevels != 256) {
        QVector<int> vals(numDesiredLevels);
        for (int i = 0; i < numDesiredLevels; i++) {
            vals[i] = i * 255.0 / float(numDesiredLevels - 1);
        }
        resampled.setIndexed(vals);
    }

    return resampled;
}
