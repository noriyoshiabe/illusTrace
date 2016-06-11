#pragma once

#include "Filter.h"

namespace illustrace {
namespace filter {

class BrightnessFilter : public Filter {
public:
    BrightnessFilter(double brightness = 0.0, double contrast = 1.0)
        : brightness(brightness), contrast(contrast) {};

    void apply(cv::Mat &image);

    double brightness;
    double contrast;
};

} // namespace filter
} // namespace illustrace
