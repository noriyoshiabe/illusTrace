#pragma once

#include "opencv2/opencv.hpp"

#include <vector>

namespace illustrace {
namespace core {

class BezierPath {
public:
    BezierPath(int segmentCount = 10, cv::Scalar color = cv::Scalar(0, 0, 0), int thickness = 1);
    ~BezierPath() {};

    void moveToPoint(const cv::Point &point);
    void curveToPoint(const cv::Point &point, const cv::Point &control1, const cv::Point &control2);
    void stroke(cv::Mat &image);

    void plotContolPointAndHandles(cv::Mat &image, std::vector<cv::Point> anchors, std::vector<cv::Point> controls);

private:
    float segmentCount;
    cv::Scalar color;
    int thickness;

    std::vector<cv::Point> anchors;
    std::vector<cv::Point> controls;
    int size;
    int length;
};

} // namespace core
} // namespace illustrace
