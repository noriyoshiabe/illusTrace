#pragma once

#include "opencv2/imgproc.hpp"

namespace illustrace {
namespace core {

class Filter {
public:
    static void brightness(cv::Mat &image, double brightness, double contrast = 1.0);
    static void blur(cv::Mat &image, int blur);
    static void threshold(cv::Mat &image);
    static void negative(cv::Mat &image);
    static void thinning(cv::Mat &image);
};

} // namespace core
} // namespace illustrace
