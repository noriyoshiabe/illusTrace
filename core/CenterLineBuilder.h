#pragma once

#include "opencv2/imgproc.hpp"
#include <vector>
#include <list>
#include <unordered_set>

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
    void build(cv::Mat &thinnedImage, std::vector<cv::KeyPoint> &keyPoints, std::vector<std::vector<cv::Point>> &results);

private:
    std::vector<cv::Point> points;
    std::unordered_set<cv::Point> startCandidates;
    std::unordered_set<cv::Point> connectingPoints;
    int width;
    int height;
    uchar **bitmap;
    uchar **checked;

    cv::Point searchStartPoint(cv::Mat &thinnedImage, cv::Point keyPoint);
    bool scan(Direction direction, std::list<cv::Point> &line, cv::Point pixel);

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
