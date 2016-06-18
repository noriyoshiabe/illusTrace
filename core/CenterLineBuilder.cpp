#include "CenterLineBuilder.h"

using namespace illustrace;
using namespace core;

void CenterLineBuilder::build(Graph &graph, std::vector<std::vector<cv::Point>> &results)
{
    for (auto *vertex : graph.vertices) {
        if (vertex->removed) {
            continue;
        }

        bool hasOpposite = 1 < vertex->adjacencyList.size();
        std::list<cv::Point> line;
        walk(vertex, line);
        if (hasOpposite) {
            line.reverse();
            line.pop_back();
            walk(vertex, line);
        }

        results.push_back(std::vector<cv::Point>(line.begin(), line.end()));
    }
}

void CenterLineBuilder::walk(GraphVertex *vertex, std::list<cv::Point> &line)
{
    line.push_back(vertex->point);

    while (!vertex->adjacencyList.empty()) {
        auto _vertex = vertex->adjacencyList[0];

        auto it = std::remove(vertex->adjacencyList.begin(), vertex->adjacencyList.end(), _vertex);
        vertex->adjacencyList.erase(it, vertex->adjacencyList.end());
        auto _it = std::remove(_vertex->adjacencyList.begin(), _vertex->adjacencyList.end(), vertex);
        _vertex->adjacencyList.erase(_it, _vertex->adjacencyList.end());
        
        if (vertex->adjacencyList.empty()) {
            vertex->removed = true;
        }
        if (_vertex->adjacencyList.empty()) {
            _vertex->removed = true;
        }

        line.push_back(_vertex->point);
        vertex = _vertex;
    }
}
