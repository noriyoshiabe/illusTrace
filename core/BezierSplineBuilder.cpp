#include "BezierSplineBuilder.h"
#include "Util.h"

using namespace illustrace;

void BezierSplineBuilder::build(std::vector<cv::Point2f> &line, Path *result, double smoothing, bool closePath)
{
    int length = line.size();

    if (1 == length) {
        result->segments.push_back(Segment::M(line[0]));
        result->closed = true;
        return;
    }

    if (2 == length) {
        result->segments.push_back(Segment::M(line[0]));
        result->segments.push_back(Segment::L(line[1]));
        return;
    }

    auto prev = Segment::M(line[0]);
    result->segments.push_back(prev);

    auto current = Segment::C(line[0], line[1], line[1]);

    for (int i = 2; i < length; ++i) {
        auto next = Segment::C(line[i-1], line[i], line[i]);
        calcControlPoint(prev, current, next, smoothing);
        result->segments.push_back(current);

        prev = current;
        current = next;
    }

    result->segments.push_back(current);
    result->closed = closePath;
}

void BezierSplineBuilder::calcControlPoint(Segment &prev, Segment &current, Segment &next, double smoothing)
{
    cv::Point2f v1 = util::vector(prev[2], current[2]);
    cv::Point2f v2 = util::vector(current[2], next[2]);

    double t = -atan2(util::crossProduct(v1, v2), util::dotProduct(v1, v2));
    double f = std::pow((fabs(t) / M_PI) - 0.8, 2.0) + 0.16; // degree of 0 to 0.8, 60 to 0.37, 90 to 0.25, 180 to 0.2

    t /= 2.0;

    double ctlNextVX = v2.x / 2.0 * f * smoothing;
    double ctlNextVY = v2.y / 2.0 * f * smoothing;
    ctlNextVX = ctlNextVX * cos(t) - ctlNextVY * sin(t);
    ctlNextVY = ctlNextVX * sin(t) + ctlNextVY * cos(t);

    next[0].x = ctlNextVX + current[2].x;
    next[0].y = ctlNextVY + current[2].y;

    double scale = util::vectorLength(v1) / util::vectorLength(v2);
    current[1].x = current[2].x - (ctlNextVX * scale);
    current[1].y = current[2].y - (ctlNextVY * scale);
}
