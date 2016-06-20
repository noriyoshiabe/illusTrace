#pragma once

#include "BezierVertex.h"

#include "opencv2/opencv.hpp"
#include <vector>

namespace illustrace {

class BezierSplineBuilder {
public:
    BezierSplineBuilder(double smoothing = 1.0) : smoothing(smoothing) {};

    void build(std::vector<cv::Point2f> &line, std::vector<BezierVertex<cv::Point2f>> &results, bool closePath = false);
    
    double smoothing;

private:
    void calcControlPoint(BezierVertex<cv::Point2f> &prev, BezierVertex<cv::Point2f> &next,
            BezierVertex<cv::Point2f> &current);
};

} // namespace illustrace
