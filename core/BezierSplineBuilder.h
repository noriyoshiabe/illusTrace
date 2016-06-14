#pragma once

#include "BezierVertex.h"

#include "opencv2/opencv.hpp"
#include <vector>

namespace illustrace {
namespace core {

class BezierSplineBuilder {
public:
    void build(std::vector<cv::Point> &line, std::vector<BezierVertex<cv::Point2f>> &result);
    
    float smoothing;
    bool closePath;
};

} // namespace core
} // namespace illustrace
