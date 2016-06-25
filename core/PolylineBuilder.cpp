#include "PolylineBuilder.h"

using namespace illustrace;

void PolylineBuilder::build(std::vector<cv::Point> &line, Path *result, bool closePath)
{
    int length = line.size();

    result->segments.push_back(Segment::M(line[0]));

    if (1 == length) {
        result->closed = true;
        return;
    }

    for (int i = 0; i < length; ++i) {
        result->segments.push_back(Segment::L(line[i]));
    }

    result->closed = closePath;
}
