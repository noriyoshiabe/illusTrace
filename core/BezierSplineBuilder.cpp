#include "BezierSplineBuilder.h"

using namespace illustrace;
using namespace core;

inline cv::Point vector(cv::Point p1, cv::Point p2)
{
    return cv::Point(p2.x - p1.x, p2.y - p1.y);
}

inline double dotProduct(cv::Point &v1, cv::Point &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

inline double vectorLength(cv::Point &p)
{
    return sqrt(p.x * p.x + p.y * p.y);
}

void BezierSplineBuilder::build(std::vector<cv::Point> &line, std::vector<BezierVertex<cv::Point2f>> &results)
{
    int length = line.size();
    int lengthMinus1 = length - 1;

    if (1 == length) {
        results.push_back(BezierVertex<cv::Point2f>(line[0], line[0], line[0]));
        results.push_back(BezierVertex<cv::Point2f>(line[0], line[0], line[0]));
        return;
    }

    if (2 == length) {
        results.push_back(BezierVertex<cv::Point2f>(line[0], line[0], line[0]));
        results.push_back(BezierVertex<cv::Point2f>(line[1], line[1], line[1]));
        return;
    }

    auto prev2 = BezierVertex<cv::Point2f>(line[0], cv::Point2f(), cv::Point2f());
    auto prev1 = BezierVertex<cv::Point2f>(line[1], cv::Point2f(), cv::Point2f());

    for (int i = 2; i < length; ++i) {
        auto current = BezierVertex<cv::Point2f>(line[i], cv::Point2f(), cv::Point2f());

        cv::Point v1 = vector(prev2.pt, prev1.pt);
        cv::Point v2 = vector(prev1.pt, current.pt);

        double cosT = dotProduct(v1, v2) / (vectorLength(v1) * vectorLength(v2));
        double t = acos(cosT);

        t /= 2.0;

        double ctlNextVX = v2.x / 2.0 * smoothing;
        double ctlNextVY = v2.y / 2.0 * smoothing;
        ctlNextVX = ctlNextVX * cos(t) - ctlNextVY * sin(t);
        ctlNextVY = ctlNextVX * sin(t) - ctlNextVY * cos(t);

        prev1.ctl.next.x = ctlNextVX + prev1.pt.x;
        prev1.ctl.next.y = ctlNextVY + prev1.pt.y;
        prev1.ctl.prev.x = prev1.pt.x - prev1.ctl.next.x;
        prev1.ctl.prev.y = prev1.pt.y - prev1.ctl.next.y;

        if (2 == i && !closePath) {
            ctlNextVX = v1.x / 2.0 * smoothing;
            ctlNextVY = v1.y / 2.0 * smoothing;
            ctlNextVX = ctlNextVX * cos(t) - ctlNextVY * sin(t);
            ctlNextVY = ctlNextVX * sin(t) - ctlNextVY * cos(t);
            prev2.ctl.next.x = ctlNextVX + prev2.pt.x;
            prev2.ctl.next.y = ctlNextVY + prev2.pt.y;
            results.push_back(prev2);
        }

        if (lengthMinus1 == i) {
            if (!closePath) {
                double ctlPrevVX = -v2.x / 2.0 * smoothing;
                double ctlPrevVY = -v2.y / 2.0 * smoothing;
                ctlPrevVX = ctlPrevVX * cos(-t) - ctlPrevVY * sin(-t);
                ctlPrevVY = ctlPrevVX * sin(-t) - ctlPrevVY * cos(-t);
                current.ctl.prev.x = ctlPrevVX + current.pt.x;
                current.ctl.prev.y = ctlPrevVY + current.pt.y;
            }
            else {
                // TODO Close Path
            }
        }

        results.push_back(prev1);

        prev2 = prev1;
        prev1 = current;
    }

    results.push_back(prev1);
}
