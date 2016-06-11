#pragma once

#include "Filter.h"

namespace illustrace {
namespace core {

class BinaryThresholdFilter : public Filter {
public:
    void apply(cv::Mat &image);
};

} // namespace core
} // namespace illustrace
