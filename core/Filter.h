#pragma once

#include "opencv2/imgproc.hpp"

namespace illustrace {

class Filter {
public:
    static void brightness(cv::Mat &image, double brightness, double contrast = 1.0);
    static void brightnessBGRA(cv::Mat &image, double brightness, double contrast = 1.0);
    static void gamma(cv::Mat &image, double g);
    static void blur(cv::Mat &image, int blur);
    static void threshold(cv::Mat &image);
    static void negative(cv::Mat &image);
    static void thinning(cv::Mat &image);
};

} // namespace illustrace
