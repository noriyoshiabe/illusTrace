#pragma once

#include "opencv2/opencv.hpp"
#include <vector>

namespace illustrace {
namespace core {

struct GraphVertex {
    GraphVertex(cv::Point point) : point(point) {};

    std::vector<GraphVertex *> adjacencyList;
    cv::Point point;
};

class Graph {
public:
    ~Graph() {
        for (auto *vertex : vertices) {
            delete vertex;
        }
    }

    std::vector<GraphVertex *> vertices;
};

class GraphBuilder {
public:
    void build(cv::Mat &thinnedImage, std::vector<cv::Point> &keyPoints, Graph &results);
};

} // namespace core
} // namespace illustrace
