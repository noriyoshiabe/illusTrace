#pragma once

#include "opencv2/opencv.hpp"
#include <vector>

namespace illustrace {
namespace core {

struct GraphVertex {
    GraphVertex(cv::Point2f point) : point(point), flag(false) {};

    std::vector<GraphVertex *> adjacencyList;
    cv::Point2f point;

    bool flag;
    int index;
};

class Graph {
public:
    ~Graph();
    Graph();
    Graph(const Graph &graph);
    Graph operator = (const Graph &graph);

    void clear();
    void dump();

    std::vector<GraphVertex *> vertices;

private:
    void copyFrom(const Graph &graph);
};

class GraphBuilder {
public:
    void build(cv::Mat &thinnedImage, std::vector<cv::Point2f> &keyPoints, Graph &results);

private:
    void mergeNearCrossPoint(Graph &results);
};

} // namespace core
} // namespace illustrace
