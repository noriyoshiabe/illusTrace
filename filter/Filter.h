#pragma once

#include "opencv2/imgproc.hpp"

namespace illustrace {
namespace filter {

class Filter {
public:
    Filter() {};
    virtual ~Filter() {};
    virtual void apply(cv::Mat &image) = 0;
};

} // namespace filter
} // namespace illustrace
