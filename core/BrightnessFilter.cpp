#include "BrightnessFilter.h" 

using namespace illustrace;
using namespace core;

cv::Mat BrightnessFilter::apply(cv::Mat &image)
{
    cv::Mat result = cv::Mat::zeros(image.size(), image.type());

    int _brightness = static_cast<int>(brightness * 100.0);

    int length = image.rows * image.cols;
    for (int i = 0; i < length; ++i) {
        result.data[i] = cv::saturate_cast<uchar>(contrast * image.data[i] + _brightness);
    }

    return result;
}
