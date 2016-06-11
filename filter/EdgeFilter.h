#pragma once

#include "Filter.h"

namespace illustrace {
namespace filter {

class EdgeFilter : public Filter {
public:
    void apply(cv::Mat &image);
};

} // namespace filter
} // namespace illustrace
