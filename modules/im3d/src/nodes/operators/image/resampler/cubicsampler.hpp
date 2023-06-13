#pragma once

#include "sampler.hpp"

namespace nitro {

    class CubicSampler : public Sampler {
    public:
        CubicSampler();

        ~CubicSampler() override;


        cv::Mat resample(const cv::Mat &colTable,
                         const std::vector<cv::Mat> &df,
                         int numDesiredLevels) override;

    protected:

        [[nodiscard]] float distFuncIndexed(const cv::Mat &colorTable,
                                            const std::vector<cv::Mat> &df,
                                            int x, int y, float p, int numLevelsInput) const override {}

    };

}  // namespace nitro
