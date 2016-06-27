#include "BezierSplineBuilder.h"
#include "Util.h"

using namespace illustrace;

void BezierSplineBuilder::build(std::vector<cv::Point2f> &line, Path *result, double smoothing, bool closePath, bool keepPoint)
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

    if (keepPoint) {
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

        if (3 <= length && result->segments[0][2] == result->segments[length - 1][2]) {
            calcControlPoint(result->segments[length - 2], result->segments[length - 1], result->segments[1], smoothing);
        }
    }
    else {
        const double CP_INTERVAL_RATE = 0.85;

        if (closePath) {
            result->segments.push_back(Segment::M(util::interval(0.5, line[0], line[1])));

            for (int i = 0; i < length; ++i) {
                int j = util::modIndex(i + 1, length);
                int k = util::modIndex(i + 2, length);

                auto p1 = util::interval(CP_INTERVAL_RATE, line[i], line[j]);
                auto p2 = util::interval(CP_INTERVAL_RATE, line[k], line[j]);
                auto p3 = util::interval(0.5, line[j], line[k]);

                result->segments.push_back(Segment::C(p1, p2, p3));
            }
        }
        else {
            result->segments.push_back(Segment::M(line[0]));
            result->segments.push_back(Segment::L(util::interval(0.5, line[0], line[1])));

            for (int i = 0; i < length - 2; ++i) {
                int j = i + 1;
                int k = i + 2;

                auto p1 = util::interval(CP_INTERVAL_RATE, line[i], line[j]);
                auto p2 = util::interval(CP_INTERVAL_RATE, line[k], line[j]);
                auto p3 = util::interval(0.5, line[j], line[k]);

                result->segments.push_back(Segment::C(p1, p2, p3));
            }

            result->segments.push_back(Segment::L(line[length - 1]));
        }
    }

    result->closed = closePath;
}

void BezierSplineBuilder::calcControlPoint(Segment &prev, Segment &current, Segment &next, double smoothing)
{
    cv::Point2f v1 = util::vector(prev[2], current[2]);
    cv::Point2f v2 = util::vector(current[2], next[2]);

    double t = -atan2(util::crossProduct(v1, v2), util::dotProduct(v1, v2));

    t /= 2.0;

    double ctlNextVX = v2.x / 4.0 * smoothing;
    double ctlNextVY = v2.y / 4.0 * smoothing;
    ctlNextVX = ctlNextVX * cos(t) - ctlNextVY * sin(t);
    ctlNextVY = ctlNextVX * sin(t) + ctlNextVY * cos(t);

    next[0].x = ctlNextVX + current[2].x;
    next[0].y = ctlNextVY + current[2].y;

    double scale = util::vectorLength(v1) / util::vectorLength(v2);
    current[1].x = current[2].x - (ctlNextVX * scale);
    current[1].y = current[2].y - (ctlNextVY * scale);
}
