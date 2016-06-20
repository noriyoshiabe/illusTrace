#pragma once

#include "GraphBuilder.h"
#include "opencv2/opencv.hpp"

#include <list>
#include <vector>

namespace illustrace {

class CenterLineBuilder {
public:
    void build(Graph &graph, std::vector<std::vector<cv::Point2f>> &results);

private:
    void walk(GraphVertex *vertex, cv::Point2f *prev, std::list<cv::Point2f> &line);
};

} // namespace illustrace
