#include "BezierPath.h"

using namespace illustrace;
using namespace core;

BezierPath::BezierPath(int segmentCount, cv::Scalar color, int thickness)
    : segmentCount(float(segmentCount)), color(color), thickness(thickness)
{
}

void BezierPath::moveToPoint(const cv::Point2f &point)
{
    anchors.clear();
    controls.clear();
    anchors.push_back(point);
}

void BezierPath::curveToPoint(const cv::Point2f &point, const cv::Point2f &control1, const cv::Point2f &control2)
{
    controls.push_back(control1);
    controls.push_back(control2);
    anchors.push_back(point);
}

void BezierPath::stroke(cv::Mat &image)
{
    // Excerpted from Drawing a cubic bezier curve using actionscript 3
    // http://blog.paultondeur.com/2008/03/09/drawing-a-cubic-bezier-curve-using-actionscript-3/
    
    float increment = 1.f / segmentCount;

    cv::Point2f a1 = anchors[0];
    cv::Point2f c1 = controls[0];

    cv::Point2f p1;
    cv::Point2f p2;

    int length = anchors.size();
    for (int i = 1; i < length; ++i) {
        cv::Point2f a2 = anchors[i];
        cv::Point2f c2 = controls[i];

        p1.x = a1.x;
        p1.y = a1.y;

        for (float u = 0.f; u <= 1.f; u += increment) {

            p2.x = std::pow(u, 3.f) * (a2.x + 3.f * (c1.x - c2.x) - a1.x)
                + 3.f * std::pow(u, 2.f) * (a1.x - 2.f * c1.x + c2.x)
                + 3.f * u * (c1.x - a1.x) + a1.x;

            p2.y = std::pow(u, 3.f) * (a2.y + 3.f * (c1.y - c2.y) - a1.y)
                + 3.f * std::pow(u, 2.f) * (a1.y - 2.f * c1.y + c2.y)
                + 3.f * u * (c1.y - a1.y) + a1.y;

            cv::line(image, p1, p2, color, thickness, CV_AA);

            p1 = p2;
        }

        cv::line(image, p2, a2, color, thickness, CV_AA);

        a1 = a2;
        c1 = c2;
    }
}

void BezierPath::plotContolPointAndHandles(cv::Mat &image)
{
    int lengthMinus1 = anchors.size() - 1;
    for (int i = 0; i < lengthMinus1; ++i) {
        cv::line(image, anchors[0], controls[0], cv::Scalar(0, 0, 255), 1, CV_AA);
        cv::line(image, anchors[1], controls[1], cv::Scalar(0, 0, 255), 1, CV_AA);
        cv::circle(image, anchors[0], 5, cv::Scalar(0, 0, 255), 1, CV_AA);
        cv::circle(image, anchors[1], 5, cv::Scalar(0, 0, 255), 1, CV_AA);
        cv::circle(image, controls[0], 5, cv::Scalar(0, 0, 255), 1, CV_AA);
        cv::circle(image, controls[1], 5, cv::Scalar(0, 0, 255), 1, CV_AA);
    }
}
