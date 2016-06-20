#pragma once

#include "Document.h"

#include "opencv2/opencv.hpp"
#include <vector>

namespace illustrace {

class BezierSplineBuilder {
public:
    static void build(std::vector<cv::Point2f> &line, Path *result, double smoothing, bool closePath = false);
private:
    static void calcControlPoint(Vertex &prev, Vertex &current, Vertex &next, double smoothing);
};

} // namespace illustrace
