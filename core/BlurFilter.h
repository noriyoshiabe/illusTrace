#pragma once

#include "Filter.h"

namespace illustrace {
namespace core {

class BlurFilter : public Filter {
public:
    BlurFilter(int blur = 5) : blur(blur) {};

    void apply(cv::Mat &image);

    int blur;
};

} // namespace core
} // namespace illustrace
