#pragma once

#include <opencv2/imgproc.hpp>

namespace nitro {

    cv::Mat signedDistField(const cv::Mat &img, int val);

    std::vector<cv::Mat> signedDistFields(const cv::Mat &img, int numLevels);
} // nitro
