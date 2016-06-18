#pragma once

#include "GraphBuilder.h"
#include "opencv2/opencv.hpp"

#include <list>
#include <vector>

namespace illustrace {
namespace core {

class CenterLineBuilder {
public:
    void build(Graph &graph, std::vector<std::vector<cv::Point>> &results);

private:
    void walk(GraphVertex *vertex, std::list<cv::Point> &line);
};

} // namespace core
} // namespace illustrace
