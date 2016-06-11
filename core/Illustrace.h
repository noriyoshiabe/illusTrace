#pragma once

#include "opencv2/imgproc.hpp"

namespace illustrace {
namespace core {

class Illustrace {
public:
    bool loadSourceImage(const char *filename);

private:
    cv::Mat sourceImage;
    cv::Mat resultImage;

    void setResultImage(cv::Mat &image);
};

} // namespace core
} // namespace illustrace
