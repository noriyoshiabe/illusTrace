#pragma once

#include "BezierVertex.h"

#include "opencv2/opencv.hpp"
#include <vector>

namespace illustrace {
namespace core {

class BezierSplineBuilder {
public:
    BezierSplineBuilder(double smoothing = 1.0, bool closePath = false)
        : smoothing(smoothing), closePath(closePath) {};

    void build(std::vector<cv::Point2f> &line, std::vector<BezierVertex<cv::Point2f>> &results);
    
    double smoothing;
    bool closePath;
};

} // namespace core
} // namespace illustrace
