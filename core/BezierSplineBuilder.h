#pragma once

#include "Document.h"

#include "opencv2/opencv.hpp"
#include <vector>

namespace illustrace {

class BezierSplineBuilder {
public:
    static void build(std::vector<cv::Point2f> &line, Path *result, double smoothing, bool closePath, bool keepPoint);
private:
    static void calcControlPoint(Segment &prev, Segment &current, Segment &next, double smoothing);
};

} // namespace illustrace
