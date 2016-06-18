#pragma once

#include "opencv2/opencv.hpp"
#include "cairo/cairo.h"

#include <vector>

namespace illustrace {
namespace core {

class BezierPath {
public:
    BezierPath(cv::Mat &image, cv::Scalar color = cv::Scalar(0, 0, 0), float thickness = 1.0f);
    ~BezierPath();

    void moveToPoint(const cv::Point2f &point);
    void curveToPoint(const cv::Point2f &point, const cv::Point2f &control1, const cv::Point2f &control2);
    void stroke();

private:
    cairo_surface_t *surface;
    cairo_t *cr;
};

} // namespace core
} // namespace illustrace
