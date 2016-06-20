#pragma once

#include "opencv2/features2d.hpp"

namespace illustrace {

class FeatureDetector {
public:
    static void detect(const cv::Mat &image, std::vector<cv::Point2f> &keyPoints);

private:
    static bool correctPoint(const cv::Mat &image, const cv::KeyPoint &keyPoint, cv::Point2f &result);
};

} // namespace illustrace
