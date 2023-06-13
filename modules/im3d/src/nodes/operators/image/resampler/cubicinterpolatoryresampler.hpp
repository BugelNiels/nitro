#pragma once

#include "resampler.hpp"

namespace nitro {

    class CubicInterpolatorySampler : public Resampler {
    public:
        CubicInterpolatorySampler();

        ~CubicInterpolatorySampler() override;


        cv::Mat resample(const cv::Mat &colTable,
                         const std::vector<cv::Mat> &df,
                         int numDesiredLevels) override;

    protected:

        [[nodiscard]] float distFuncIndexed(const cv::Mat &colorTable,
                                            const std::vector<cv::Mat> &df,
                                            int x, int y, float p, int numLevelsInput) const override {}

    };

}  // namespace nitro
