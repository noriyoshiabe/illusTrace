#include "BezierSplineBuilder.h"
#include "Util.h"

using namespace illustrace;

void BezierSplineBuilder::build(std::vector<cv::Point2f> &line, Path *result, double smoothing, bool closePath)
{
    int length = line.size();
    int lengthMinus1 = length - 1;

    if (1 == length) {
        result->vertices.push_back(Vertex(line[0], line[0], line[0]));
        result->vertices.push_back(Vertex(line[0], line[0], line[0]));
        return;
    }

    if (2 == length) {
        result->vertices.push_back(Vertex(line[0], line[0], line[0]));
        result->vertices.push_back(Vertex(line[1], line[1], line[1]));
        return;
    }


    for (int i = 0; i < length; ++i) {
        auto current = Vertex(line[i], line[i], line[i]);

        if (0 < i && i < lengthMinus1) {
            auto prev = Vertex(line[i-1], line[i-1], line[i-1]);
            auto next = Vertex(line[i+1], line[i+1], line[i+1]);
            calcControlPoint(prev, current, next, smoothing);
        }

        result->vertices.push_back(current);
    }

    if (closePath && 3 <= length) {
        calcControlPoint(result->vertices[lengthMinus1 - 1], result->vertices[lengthMinus1], result->vertices[0], smoothing);
        calcControlPoint(result->vertices[lengthMinus1], result->vertices[0], result->vertices[1], smoothing);
        result->closed = true;
    }
}

void BezierSplineBuilder::calcControlPoint(Vertex &prev, Vertex &current, Vertex &next, double smoothing)
{
    cv::Point2f v1 = util::vector(prev.p, current.p);
    cv::Point2f v2 = util::vector(current.p, next.p);

    double t = -atan2(util::crossProduct(v1, v2), util::dotProduct(v1, v2));
    double f = std::pow((fabs(t) / M_PI) - 0.8, 2.0) + 0.16; // degree of 0 to 0.8, 60 to 0.37, 90 to 0.25, 180 to 0.2

    t /= 2.0;

    double ctlNextVX = v2.x / 2.0 * f * smoothing;
    double ctlNextVY = v2.y / 2.0 * f * smoothing;
    ctlNextVX = ctlNextVX * cos(t) - ctlNextVY * sin(t);
    ctlNextVY = ctlNextVX * sin(t) + ctlNextVY * cos(t);

    current.c.next.x = ctlNextVX + current.p.x;
    current.c.next.y = ctlNextVY + current.p.y;

    double scale = util::vectorLength(v1) / util::vectorLength(v2);
    current.c.prev.x = current.p.x - (ctlNextVX * scale);
    current.c.prev.y = current.p.y - (ctlNextVY * scale);
}
