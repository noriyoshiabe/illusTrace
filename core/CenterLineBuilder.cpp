#include "CenterLineBuilder.h"
#include "Vector2D.h"

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
        walk(vertex, nullptr, line);
        if (hasOpposite) {
            line.reverse();
            line.pop_back();
            walk(vertex, &line.back(), line);
        }

        results.push_back(std::vector<cv::Point>(line.begin(), line.end()));
    }
}

void CenterLineBuilder::walk(GraphVertex *vertex, cv::Point *prev, std::list<cv::Point> &line)
{
    line.push_back(vertex->point);

    while (!vertex->adjacencyList.empty()) {
        GraphVertex *_vertex;

        int length = vertex->adjacencyList.size();
        if (1 < length && prev) {
            double minRadian = 999.0;
            int minIndex = 0;
            for (int i = 0; i < length; ++i) {
                cv::Point v1 = lib::vector(*prev, vertex->point);
                cv::Point v2 = lib::vector(vertex->point, vertex->adjacencyList[i]->point);
                double lv1 = lib::vectorLength(v1);
                double lv2 = lib::vectorLength(v2);
                double radian = acos(lib::dotProduct(v1, v2) / (lv1 * lv2));
                if (minRadian > radian) {
                    minRadian = radian;
                    minIndex = i;
                }
            }
            _vertex = vertex->adjacencyList[minIndex];
        }
        else {
            _vertex = vertex->adjacencyList[0];
        }

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
        prev = &vertex->point;
        vertex = _vertex;
    }
}
