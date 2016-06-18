#pragma once

#include "opencv2/opencv.hpp"
#include <vector>

namespace illustrace {
namespace core {

struct GraphVertex {
    GraphVertex(cv::Point point) : point(point) {};

    std::vector<GraphVertex *> adjacencyList;
    cv::Point point;

    bool removed;
};

class Graph {
public:
    ~Graph();
    void clear();

    std::vector<GraphVertex *> vertices;
};

class GraphBuilder {
public:
    void build(cv::Mat &thinnedImage, std::vector<cv::Point> &keyPoints, Graph &results);
};

} // namespace core
} // namespace illustrace
