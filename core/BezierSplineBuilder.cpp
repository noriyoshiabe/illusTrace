#include "BezierSplineBuilder.h"
#include "Util.h"

using namespace illustrace;

void BezierSplineBuilder::build(std::vector<cv::Point2f> &line, std::vector<BezierVertex<cv::Point2f>> &results, bool closePath)
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


    for (int i = 0; i < length; ++i) {
        auto current = BezierVertex<cv::Point2f>(line[i], line[i], line[i]);

        if (0 < i && i < lengthMinus1) {
            auto prev = BezierVertex<cv::Point2f>(line[i-1], line[i-1], line[i-1]);
            auto next = BezierVertex<cv::Point2f>(line[i+1], line[i+1], line[i+1]); 
            calcControlPoint(prev, next, current);
        }

        results.push_back(current);
    }

    if (closePath && 3 <= length) {
        calcControlPoint(results.back(), results[1], results[0]);
        results.push_back(results[0]);
    }
}

void BezierSplineBuilder::calcControlPoint(BezierVertex<cv::Point2f> &prev, BezierVertex<cv::Point2f> &next,
        BezierVertex<cv::Point2f> &current)
{
    cv::Point v1 = util::vector(prev.pt, current.pt);
    cv::Point v2 = util::vector(current.pt, next.pt);

    double t = -atan2(util::crossProduct(v1, v2), util::dotProduct(v1, v2));
    double f = std::pow((fabs(t) / M_PI) - 0.8, 2.0) + 0.16; // degree of 0 to 0.8, 60 to 0.37, 90 to 0.25, 180 to 0.2

    t /= 2.0;

    double ctlNextVX = v2.x / 2.0 * f * smoothing;
    double ctlNextVY = v2.y / 2.0 * f * smoothing;
    ctlNextVX = ctlNextVX * cos(t) - ctlNextVY * sin(t);
    ctlNextVY = ctlNextVX * sin(t) + ctlNextVY * cos(t);

    current.ctl.next.x = ctlNextVX + current.pt.x;
    current.ctl.next.y = ctlNextVY + current.pt.y;

    double scale = util::vectorLength(v1) / util::vectorLength(v2);
    current.ctl.prev.x = current.pt.x - (ctlNextVX * scale);
    current.ctl.prev.y = current.pt.y - (ctlNextVY * scale);
}
