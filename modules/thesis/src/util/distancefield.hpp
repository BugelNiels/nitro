#pragma once

#include <opencv2/imgproc.hpp>

namespace nitro::Thesis {

cv::Mat signedDistField(const cv::Mat &img, int val);

} // namespace nitro::Thesis
