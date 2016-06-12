#include "CenterLineBuilder.h"

#include <iostream>

using namespace illustrace;
using namespace core;

void CenterLineBuilder::build(cv::Mat &thinnedImage, std::vector<cv::KeyPoint> &keyPoints, std::vector<std::vector<cv::Point>> &results)
{
    width = thinnedImage.cols;
    height = thinnedImage.rows;

    for (auto keyPoint : keyPoints) {
        cv::Point point = cv::Point(keyPoint.pt.x, keyPoint.pt.y);
        points.push_back(point);
        startCandidates.insert(point);
    }

    bitmap = new uchar*[height];
    checked = new uchar*[height];

    for (int y = 0; y < height; ++y) {
        bitmap[y] = new uchar[width];
        checked[y] = new uchar[width];

        int yOffset = y * width;
        for (int x = 0; x < width; ++x) {
            bitmap[y][x] = thinnedImage.data[yOffset + x];
        }
    }

    while (!startCandidates.empty()) {
        auto it = startCandidates.begin();
        cv::Point point = *it;
        startCandidates.erase(it);

        cv::Point pixel = searchStartPoint(thinnedImage, point);
        if (-1 == pixel.x) {
            continue;
        }

        connectingPoints.clear();

        std::list<cv::Point> line;
        line.push_back(point);
        connectingPoints.insert(point);

        checked[pixel.y][pixel.x] = 1;
        bool connected = false;

        for (int direction = 0; direction < 8; ++direction) {
            if (scan(static_cast<Direction>(direction), line, pixel)) {
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
        delete checked[y];
    }

    delete bitmap;
    delete checked;

    points.clear();
    startCandidates.clear();
    connectingPoints.clear();
}

cv::Point CenterLineBuilder::searchStartPoint(cv::Mat &thinnedImage, cv::Point keyPoint)
{
    int yFrom = MAX(0, keyPoint.y - 2);
    int yTo = MIN(thinnedImage.rows - 1, keyPoint.y + 2);
    int xFrom = MAX(0, keyPoint.x - 2);
    int xTo = MIN(thinnedImage.cols - 1, keyPoint.x + 2);

    for (int y = yFrom; y < yTo; ++y) {
        int yOffset = thinnedImage.cols * y;
        for (int x = xFrom; x < xTo; ++x) {
            if (0 == thinnedImage.data[yOffset + x]) {
                return cv::Point(x, y);
            }
        }
    }

    return cv::Point(-1, -1);
}

bool CenterLineBuilder::scan(Direction direction, std::list<cv::Point> &line, cv::Point pixel)
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
            int x = coordTable[static_cast<int>(toScan)].x + pixel.x;
            int y = coordTable[static_cast<int>(toScan)].y + pixel.y;

            if (x < 0 || width <= x || y < 0 || height <= y) {
                continue;
            }

            if (0 != bitmap[y][x] || checked[y][x]) {
                continue;
            }
            checked[y][x] = 1;

            pixel.x = x;
            pixel.y = y;
            direction = toScan;

            auto it = std::find_if(points.begin(), points.end(), [&](const cv::Point &point) {
                return 3 > std::abs(point.x - x) && 3 > std::abs(point.y - y);
            });

            if (it != points.end()) {
                if (connectingPoints.find(*it) == connectingPoints.end()
                        || (1 < line.size() && *it == *line.begin())) {
                    line.push_back(*it);
                    connectingPoints.insert(*it);
                    startCandidates.erase(*it);
                    connected = true;
                }
            }

            break;
        }
    }

    return connected;
}
