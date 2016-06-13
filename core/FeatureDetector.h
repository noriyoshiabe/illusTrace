#pragma once

#include "opencv2/features2d.hpp"

namespace illustrace {
namespace core {

class FeatureDetector {
public:
    void detect(const cv::Mat &image, std::vector<cv::Point> &keyPoints);

private:
    bool correctPoint(const cv::Mat &image, const cv::KeyPoint &keyPoint, cv::Point &result);
};

} // namespace core
} // namespace illustrace
