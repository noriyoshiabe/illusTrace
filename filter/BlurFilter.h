#pragma once

#include "Filter.h"

namespace illustrace {
namespace filter {

class BlurFilter : public Filter {
public:
    BlurFilter(int blur = 5) : blur(blur) {};

    void apply(cv::Mat &image);

    int blur;
};

} // namespace filter
} // namespace illustrace
