#include "GraphBuilder.h"
#include "Vector2D.h"

using namespace illustrace;
using namespace core;

Graph::~Graph()
{
    clear();
}

void Graph::clear()
{
    for (auto *vertex : vertices) {
        delete vertex;
    }
    vertices.clear();
}

void Graph::dump()
{
    for (auto *vertex : vertices) {
        printf("point=[%f,%f] adjacency=[", vertex->point.x, vertex->point.y);
        bool needSep = false;
        for (auto *_vertex : vertex->adjacencyList) {
            if (needSep) {
                printf(",");
            }
            needSep = true;
            auto it = std::find(vertices.begin(), vertices.end(), _vertex);
            size_t index = std::distance(vertices.begin(), it);
            printf("%lu", index);
        }
        printf("] removed=%s\n", vertex->removed ? "true" : "false");
    }
}

enum class Direction : int {
    North = 0,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest
};

inline Direction directionToScan(Direction current, int stage) {
    const Direction table[] = {
        Direction::North,
        Direction::NorthEast,
        Direction::NorthWest,
        Direction::East,
        Direction::West,
        Direction::SouthEast,
        Direction::SouthWest,
        Direction::South,
    };

    Direction directionForNorth = table[stage];
    int direction = static_cast<int>(directionForNorth) + static_cast<int>(current);
    if (8 <= direction) {
        direction -= 8;
    }
    return static_cast<Direction>(direction);
}

void GraphBuilder::build(cv::Mat &thinnedImage, std::vector<cv::Point2f> &keyPoints, Graph &results)
{
    results.clear();

    int width = thinnedImage.cols;
    int height = thinnedImage.rows;

    uchar **bitmap = new uchar*[height];
    GraphVertex ***vertexMap = new GraphVertex**[height];
    GraphVertex ***vertexHist = new GraphVertex**[height];

    for (int y = 0; y < height; ++y) {
        bitmap[y] = new uchar[width];
        vertexMap[y] = new GraphVertex*[width];
        vertexHist[y] = new GraphVertex*[width];

        int yOffset = y * width;
        for (int x = 0; x < width; ++x) {
            bitmap[y][x] = thinnedImage.data[yOffset + x];
            vertexMap[y][x] = nullptr;
            vertexHist[y][x] = nullptr;
        }
    }

    for (auto &keyPoint : keyPoints) {
        results.vertices.push_back(new GraphVertex(keyPoint));
    }

    for (auto *vertex : results.vertices) {
        for (int y = vertex->point.y - 2; y <= vertex->point.y + 2; ++y) {
            for (int x = vertex->point.x - 2; x <= vertex->point.x + 2; ++x) {
                if (0 <= y && y < height && 0 <= x && x < width) {
                    vertexMap[y][x] = vertex;
                }
            }
        }
    }

    for (auto *vertex : results.vertices) {
        const struct {
            int x;
            int y;
        } coordTable[8] = {
            {-1,  0}, // West
            {-1, -1}, // NorthWest
            { 0, -1}, // North
            { 1, -1}, // NorthEast
            { 1,  0}, // East
            { 1,  1}, // SouthEast
            { 0,  1}, // South
            {-1,  1}, // SouthWest
        };

        for (int i = 0; i < 2; ++i) {
            int x = vertex->point.x;
            int y = vertex->point.y;
            Direction direction = Direction::North;

            int stage = 0;
            while (8 > stage) {
                for (stage = 0; stage < 8; ++stage) {
                    Direction toScan = directionToScan(direction, stage);
                    int _x = coordTable[static_cast<int>(toScan)].x + x;
                    int _y = coordTable[static_cast<int>(toScan)].y + y;

                    if (_x < 0 || width <= _x || _y < 0 || height <= _y) {
                        continue;
                    }

                    if (0 != bitmap[_y][_x]) {
                        continue;
                    }

                    if (vertex == vertexHist[_y][_x]) {
                        continue;
                    }

                    x = _x;
                    y = _y;
                    vertexHist[y][x] = vertex;
                    direction = toScan;

                    GraphVertex *pv = vertexMap[y][x];
                    if (pv && vertex != pv) {
                        if (pv->adjacencyList.end() == std::find(pv->adjacencyList.begin(), pv->adjacencyList.end(), vertex)) {
                            vertex->adjacencyList.push_back(pv);
                            pv->adjacencyList.push_back(vertex);
                        }
                        stage = 8;
                    }

                    break;
                }
            }
        }
    }

    mergeNearCrossPoint(results);

    for (int y = 0; y < height; ++y) {
        delete bitmap[y];
        delete vertexMap[y];
        delete vertexHist[y];
    }

    delete[] bitmap;
    delete[] vertexMap;
    delete[] vertexHist;

#if 0
    results.dump();
#endif
}

void GraphBuilder::mergeNearCrossPoint(Graph &results)
{
    std::vector<GraphVertex *> crossPoints;

    for (auto *vertex : results.vertices) {
        if (3 <= vertex->adjacencyList.size()) {
            crossPoints.push_back(vertex);
        }
    }

    for (auto *vertex : crossPoints) {
        if (vertex->removed) {
            continue;
        }

        std::vector<GraphVertex *> toMerge;

        for (auto *_vertex : vertex->adjacencyList) {
            if (3 <= _vertex->adjacencyList.size()) {
                if (5.0 > lib::vectorLength(lib::vector(vertex->point, _vertex->point))) {
                    toMerge.push_back(_vertex);
                }
            }
        }

        if (!toMerge.empty()) {
            for (auto *_vertex : toMerge) {
                auto it = std::remove(vertex->adjacencyList.begin(), vertex->adjacencyList.end(), _vertex);
                vertex->adjacencyList.erase(it, vertex->adjacencyList.end());
                auto _it = std::remove(_vertex->adjacencyList.begin(), _vertex->adjacencyList.end(), vertex);
                _vertex->adjacencyList.erase(_it, _vertex->adjacencyList.end());

                vertex->point.x += _vertex->point.x;
                vertex->point.y += _vertex->point.y;

                for (auto *adjacency : _vertex->adjacencyList) {
                    vertex->adjacencyList.push_back(adjacency);
                    adjacency->adjacencyList.push_back(vertex);

                    auto __it = std::remove(adjacency->adjacencyList.begin(), adjacency->adjacencyList.end(), _vertex);
                    adjacency->adjacencyList.erase(__it, adjacency->adjacencyList.end());
                }

                _vertex->removed = true;
            }

            int division = toMerge.size() + 1;
            vertex->point.x /= division;
            vertex->point.y /= division;
        }
    }

}
