#include "CenterLineBuilder.h"

#include <iostream>

using namespace illustrace;
using namespace core;

void CenterLineBuilder::build(cv::Mat &thinnedImage, std::vector<std::vector<cv::Point>> &results)
{
    width = thinnedImage.cols;
    height = thinnedImage.rows;

    bitmap = new uchar*[height];
    for (int y = 0; y < height; ++y) {
        bitmap[y] = new uchar[width];

        int yOffset = y * width;
        for (int x = 0; x < width; ++x) {
            bitmap[y][x] = thinnedImage.data[yOffset + x];
        }
    }

    cv::Point point;

    while (searchStartPoint(point)) {
        std::list<cv::Point> line;
        line.push_back(point);
        connectedPoints.push_back(point);
        bitmap[point.y][point.x] = 128;

        bool connected = false;

        for (int direction = 0; direction < 8; ++direction) {
            if (scan(static_cast<Direction>(direction), line, point)) {
                results.push_back(std::vector<cv::Point>(line.begin(), line.end()));
                line = std::list<cv::Point>();
                line.push_back(point);
                connectedPoints.push_back(point);
                connected = true;
            }
        }

        if (!connected) {
            results.push_back(std::vector<cv::Point>(line.begin(), line.end()));
        }
    }

    for (int i = results.size() - 1; 0 <= i; --i) {
        auto line = results[i];
        if (1 == line.size()) {
            cv::Point _point = line[0];

            auto fit = std::find_if(connectedPoints.begin(), connectedPoints.end(), [&](const cv::Point &point) {
                return point.x - 2 <= _point.x && _point.x <= point.x + 2
                    && point.y - 2 <= _point.y && _point.y <= point.y + 2
                    && point.x != _point.x && _point.y != point.y;
            });

            if (fit != connectedPoints.end()) {
                results.erase(results.begin() + i);
            }
        }
    }

    for (int y = 0; y < height; ++y) {
        delete bitmap[y];
    }
    delete bitmap;

    connectedLines.clear();
}

bool CenterLineBuilder::searchStartPoint(cv::Point &point)
{
    for (int y = checkedY; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (0 == bitmap[y][x]) {
                point.x = x;
                point.y = y;
                return true;
            }
        }
        ++checkedY;
    }

    return false;
}

bool CenterLineBuilder::scan(Direction direction, std::list<cv::Point> &line, cv::Point point)
{
    bool connected = false;

    const struct {
        int x;
        int y;
    } coordTable[8] = {
        { 0, -1}, // North
        {-1, -1}, // NorthWest
        { 1, -1}, // NorthEast
        {-1,  0}, // West
        { 1,  0}, // East
        { 1,  1}, // SouthEast
        {-1,  1}, // SouthWest
        { 0,  1}, // South
    };

    cv::Point startPoint = point;
    bool reversed = false;

REVERSE:
    int stage = 0;

    while (stage < 8) {
        for (stage = 0; stage < 8; ++stage) {
            Direction toScan = directionToScan(direction, stage);
            int x = coordTable[static_cast<int>(toScan)].x + point.x;
            int y = coordTable[static_cast<int>(toScan)].y + point.y;

            if (x < 0 || width <= x || y < 0 || height <= y) {
                continue;
            }

            if (0 != bitmap[y][x]) {
                continue;
            }
            bitmap[y][x] = 128;
            
            cv::Point prev = point;
            point.x = x;
            point.y = y;

            direction = toScan;

            ConnectedLine connectedLine = ConnectedLine(prev, point);
            if (connectedLines.find(connectedLine) == connectedLines.end()) {
                line.push_back(point);
                connectedPoints.push_back(point);
                connectedLines.insert(connectedLine);
                connected = true;
            }

            break;
        }
    }

    if (!reversed) {
        line.reverse();
        point = startPoint;
        reversed = true;
        goto REVERSE;
    }

    return connected;
}
