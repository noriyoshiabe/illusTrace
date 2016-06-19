#pragma once

#include "opencv2/opencv.hpp"
#include <vector>

namespace illustrace {
namespace core {

struct GraphVertex {
    GraphVertex(cv::Point2f point) : point(point), removed(false) {};

    std::vector<GraphVertex *> adjacencyList;
    cv::Point2f point;

    bool removed;
};

class Graph {
public:
    ~Graph();
    void clear();
    void dump();

    std::vector<GraphVertex *> vertices;
};

class GraphBuilder {
public:
    void build(cv::Mat &thinnedImage, std::vector<cv::Point2f> &keyPoints, Graph &results);

private:
    void mergeNearCrossPoint(Graph &results);
};

} // namespace core
} // namespace illustrace
