#pragma once

#include "Filter.h"

namespace illustrace {
namespace filter {

class NegativeFilter : public Filter {
public:
    void apply(cv::Mat &image);
};

} // namespace filter
} // namespace illustrace
