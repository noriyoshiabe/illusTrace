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
        cv::Point2f a1 = anchors[i-1];
        cv::Point2f a2 = anchors[i];
        cv::Point2f c1 = controls[(i-1)*2];
        cv::Point2f c2 = controls[(i-1)*2+1];

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
    }
}

void BezierPath::plotContolPointAndHandles(cv::Mat &image)
{
    int length = anchors.size();
    for (int i = 1; i < length; ++i) {
        cv::Point2f a1 = anchors[i-1];
        cv::Point2f a2 = anchors[i];
        cv::Point2f c1 = controls[(i-1)*2];
        cv::Point2f c2 = controls[(i-1)*2+1];

        cv::line(image, a1, c1, cv::Scalar(128, 0, 0), 1, CV_AA);
        cv::line(image, a2, c2, cv::Scalar(128, 0, 0), 1, CV_AA);
        cv::circle(image, a1, 5, cv::Scalar(128, 0, 0), 1, CV_AA);
        cv::circle(image, a2, 5, cv::Scalar(128, 0, 0), 1, CV_AA);
        cv::circle(image, c1, 5, cv::Scalar(128, 0, 0), 1, CV_AA);
        cv::circle(image, c2, 5, cv::Scalar(128, 0, 0), 1, CV_AA);
    }
}
