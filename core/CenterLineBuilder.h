#pragma once

#include "opencv2/imgproc.hpp"
#include <vector>
#include <list>
#include <unordered_set>

namespace illustrace {
namespace core {

struct ConnectedLine {
    cv::Point start;
    cv::Point end;

    ConnectedLine(cv::Point start, cv::Point end) : start(start), end(end) {};

    bool operator == (ConnectedLine const &other) const {
        return this->start == other.start && this->end == other.end;
    }
};

} // namespace core
} // namespace illustrace

namespace std {

template<>
struct hash<cv::Point> {
    size_t operator()(const cv::Point &pt) const {
        size_t hash = 17;
        hash = ((hash + pt.x) << 5) - (hash + pt.x);
        hash = ((hash + pt.y) << 5) - (hash + pt.y);
        return hash;
    }
};

template<>
struct hash<illustrace::core::ConnectedLine> {
    size_t operator()(const illustrace::core::ConnectedLine &line) const {
        return std::hash<cv::Point>()(line.start) ^ std::hash<cv::Point>()(line.end);
    }
};

}

namespace illustrace {
namespace core {

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

class CenterLineBuilder {
public:
    void build(cv::Mat &thinnedImage, std::vector<cv::Point> keyPoints, std::vector<std::vector<cv::Point>> &results);

private:
    std::unordered_set<ConnectedLine> connectedLines;
    int width;
    int height;
    uchar **bitmap;
    int checkedY;

    bool searchStartPoint(cv::Point &point);
    bool scan(Direction direction, std::list<cv::Point> &line, cv::Point point);

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
};

} // namespace core
} // namespace illustrace
