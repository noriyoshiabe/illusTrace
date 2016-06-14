#pragma once

#include "opencv2/opencv.hpp"

#include <vector>

namespace illustrace {
namespace core {

class BezierPath {
public:
    BezierPath(int segmentCount = 10, cv::Scalar color = cv::Scalar(0, 0, 0), int thickness = 1);
    ~BezierPath() {};

    void moveToPoint(const cv::Point2f &point);
    void curveToPoint(const cv::Point2f &point, const cv::Point2f &control1, const cv::Point2f &control2);
    void stroke(cv::Mat &image);

    void plotContolPointAndHandles(cv::Mat &image, std::vector<cv::Point2f> anchors, std::vector<cv::Point2f> controls);

    float segmentCount;
    cv::Scalar color;
    int thickness;

private:
    std::vector<cv::Point2f> anchors;
    std::vector<cv::Point2f> controls;
    int size;
    int length;
};

} // namespace core
} // namespace illustrace
