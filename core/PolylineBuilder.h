#pragma once

#include "Document.h"

#include "opencv2/opencv.hpp"
#include <vector>

namespace illustrace {

class PolylineBuilder {
public:
    static void build(std::vector<cv::Point> &line, Path *result, bool closePath = false);
};

} // namespace illustrace
