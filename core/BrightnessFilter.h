#pragma once

#include "Filter.h"

namespace illustrace {
namespace core {

class BrightnessFilter : public Filter {
public:
    BrightnessFilter(double brightness = 0.0, double contrast = 1.0)
        : brightness(brightness), contrast(contrast) {};

    void apply(cv::Mat &image);

    double brightness;
    double contrast;
};

} // namespace core
} // namespace illustrace
