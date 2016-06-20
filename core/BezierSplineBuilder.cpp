#include "BezierSplineBuilder.h"
#include "Util.h"

using namespace illustrace;

void BezierSplineBuilder::build(std::vector<cv::Point2f> &line, Path *result, double smoothing, bool closePath)
{
    int length = line.size();
    int lengthMinus1 = length - 1;

    if (1 == length) {
        result->segments.push_back(Segment(line[0]));
        return;
    }

    if (2 == length) {
        result->segments.push_back(Segment(line[0], line[1]));
        return;
    }


    for (int i = 0; i < length; ++i) {
        auto current = Segment(line[i], line[i], line[i]);

        if (0 < i && i < lengthMinus1) {
            auto prev = Segment(line[i-1], line[i-1], line[i-1]);
            auto next = Segment(line[i+1], line[i+1], line[i+1]);
            calcControlPoint(prev, current, next, smoothing);
        }

        result->segments.push_back(current);
    }

    if (closePath && 3 <= length) {
        calcControlPoint(result->segments.back(), result->segments[0], result->segments[1], smoothing);
        result->closed = true;
    }
}

void BezierSplineBuilder::calcControlPoint(Segment &prev, Segment &current, Segment &next, double smoothing)
{
    cv::Point2f v1 = util::vector(prev.p1, current.p1);
    cv::Point2f v2 = util::vector(current.p1, next.p1);

    double t = -atan2(util::crossProduct(v1, v2), util::dotProduct(v1, v2));
    double f = std::pow((fabs(t) / M_PI) - 0.8, 2.0) + 0.16; // degree of 0 to 0.8, 60 to 0.37, 90 to 0.25, 180 to 0.2

    t /= 2.0;

    double ctlNextVX = v2.x / 2.0 * f * smoothing;
    double ctlNextVY = v2.y / 2.0 * f * smoothing;
    ctlNextVX = ctlNextVX * cos(t) - ctlNextVY * sin(t);
    ctlNextVY = ctlNextVX * sin(t) + ctlNextVY * cos(t);

    current.p2.x = ctlNextVX + current.p1.x;
    current.p2.y = ctlNextVY + current.p1.y;

    double scale = util::vectorLength(v1) / util::vectorLength(v2);
    prev.p3.x = current.p1.x - (ctlNextVX * scale);
    prev.p3.y = current.p1.y - (ctlNextVY * scale);
}
