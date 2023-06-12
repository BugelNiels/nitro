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


        virtual float distFuncIndexed(const cv::Mat &colorTable,
                                      const std::vector<cv::Mat> &df,
                                      int x, int y, float p, int numLevelsInput,
                                      std::vector<float> &tangents) const;

    private:
        void computeMonotonicTangents(const cv::Mat &colorTable,
                                      const std::vector<cv::Mat> &df,
                                      int x, int y,
                                      std::vector<float> &tangents,
                                      std::vector<float> &secants);

    };

}  // namespace nitro
