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
        bitmap[point.y][point.x] = 128;

        bool connected = false;

        for (int direction = 0; direction < 8; ++direction) {
            if (scan(static_cast<Direction>(direction), line, point)) {
                results.push_back(std::vector<cv::Point>(line.begin(), line.end()));
                line = std::list<cv::Point>();
                line.push_back(point);
                connected = true;
            }
        }

        if (!connected) {
            results.push_back(std::vector<cv::Point>(line.begin(), line.end()));
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
        { 0, -1},
        { 1, -1},
        { 1,  0},
        { 1,  1},
        { 0,  1},
        {-1,  1},
        {-1,  0},
        {-1, -1},
    };

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
                connectedLines.insert(connectedLine);
                connected = true;
            }

            break;
        }
    }

    return connected;
}
