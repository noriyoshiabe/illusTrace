#pragma once

#include "opencv2/imgproc.hpp"

namespace illustrace {
namespace core {

class Filter {
public:
    Filter() {};
    virtual ~Filter() {};
    virtual void apply(cv::Mat &image) = 0;
};

} // namespace core
} // namespace illustrace
