#include "CenterLineBuilder.h"
#include "Vector2D.h"

using namespace illustrace;
using namespace core;

void CenterLineBuilder::build(Graph &graph, std::vector<std::vector<cv::Point2f>> &results)
{
    for (auto *vertex : graph.vertices) {
        if (vertex->flag) {
            continue;
        }

        bool hasOpposite = 1 < vertex->adjacencyList.size();
        std::list<cv::Point2f> line;
        walk(vertex, nullptr, line);
        if (hasOpposite) {
            line.reverse();
            line.pop_back();
            walk(vertex, &line.back(), line);
        }

        results.push_back(std::vector<cv::Point2f>(line.begin(), line.end()));
    }
}

void CenterLineBuilder::walk(GraphVertex *vertex, cv::Point2f *prev, std::list<cv::Point2f> &line)
{
    line.push_back(vertex->point);

    while (!vertex->adjacencyList.empty()) {
        GraphVertex *_vertex;

        int length = vertex->adjacencyList.size();
        if (1 < length && prev) {
            double minRadian = 999.0;
            int minIndex = 0;
            
            for (int i = 0; i < length; ++i) {
                double radian = lib::cornerRadian(*prev, vertex->point, vertex->adjacencyList[i]->point);
                if (minRadian > radian) {
                    minRadian = radian;
                    minIndex = i;
                }
            }

            for (int i = 0; i < length; ++i) {
                for (int j = i + 1; j < length; ++j) {
                    double radian = lib::cornerRadian(vertex->adjacencyList[i]->point, vertex->point, vertex->adjacencyList[j]->point);
                    if (minRadian > radian) {
                        return;
                    }
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
            vertex->flag = true;
        }
        if (_vertex->adjacencyList.empty()) {
            _vertex->flag = true;
        }

        line.push_back(_vertex->point);
        prev = &vertex->point;
        vertex = _vertex;
    }
}
