#pragma once

#include "opencv2/opencv.hpp"
#include <vector>

namespace illustrace {

struct GraphVertex {
    GraphVertex(cv::Point2f point)
        : point(point), index(0), removed(false), flag(false), weight(0.f) {};

    std::vector<GraphVertex *> adjacencyList;
    cv::Point2f point;

    int index;
    bool removed;
    bool flag;
    float weight;
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
    static void build(cv::Mat &thinnedImage, std::vector<cv::Point2f> &keyPoints, Graph &result);
    static void approximate(const Graph &graph, Graph &resul);

private:
    static void mergeNearCrossPoint(Graph &result);
};

} // namespace illustrace
