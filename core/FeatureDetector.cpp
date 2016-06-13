#include "FeatureDetector.h"

using namespace illustrace;
using namespace core;

void FeatureDetector::detect(const cv::Mat &image, std::vector<cv::Point> &keyPoints)
{
    std::vector<cv::KeyPoint> _keyPoints;

    cv::Ptr<cv::GFTTDetector> detector = cv::GFTTDetector::create(0, 0.01, 2);
    detector->detect(image, _keyPoints);

    for (auto keyPoint : _keyPoints) {
        int x = keyPoint.pt.x;
        int y = keyPoint.pt.y;
        if (0 == image.data[image.cols * y + x]) {
            keyPoints.push_back(cv::Point(x, y));
        }
        else {
            cv::Point point;
            if (correctPoint(image, keyPoint, point)) {
                keyPoints.push_back(point);
            }
        }
    }

    std::sort(keyPoints.begin(), keyPoints.end(), [](const cv::Point &a, const cv::Point &b) {
        return a.x == b.x ? a.y < b.y : a.x < b.x;
    });

    int size = keyPoints.size();
    for (int i = 1; i < size; ++i) {
        if (keyPoints[i - 1] == keyPoints[i]) {
            keyPoints.erase(keyPoints.begin() + i);
            --i;
            --size;
        }
    }
}

bool FeatureDetector::correctPoint(const cv::Mat &image, const cv::KeyPoint &keyPoint, cv::Point &result)
{
    for (int i = 0; i < 24; ++i) {
        int table[24][2] = {
            { 0, -1}, // 0
            { 1,  0}, // 1
            { 0,  1}, // 2
            {-1,  0}, // 3
            { 1, -1}, // 4
            { 1,  1}, // 5
            {-1,  1}, // 6
            {-1, -1}, // 7
            { 0, -2}, // 8
            { 2,  0}, // 9
            { 0,  2}, // 10
            {-2,  0}, // 11
            { 1, -2}, // 12
            { 2, -1}, // 13
            { 2,  1}, // 14
            { 1,  2}, // 15
            {-1,  2}, // 16
            {-2,  1}, // 17
            {-2, -1}, // 18
            {-1, -2}, // 19
            { 2, -2}, // 20
            { 2,  2}, // 21
            {-2,  2}, // 22
            {-2, -2}, // 23
        };

        int x = table[i][0] + keyPoint.pt.x;
        int y = table[i][1] + keyPoint.pt.y;

        if (0 <= x && x < image.cols && 0 <= y && y < image.rows) {
            if (0 == image.data[y * image.cols + x]) {
                result.x = x;
                result.y = y;
                return true;
            }
        }
    }

    return false;
}
