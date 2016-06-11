#include "BrightnessFilter.h" 

using namespace illustrace;
using namespace core;

void BrightnessFilter::apply(cv::Mat &image)
{
    int _brightness = static_cast<int>(brightness * 100.0);

    int length = image.rows * image.cols;
    for (int i = 0; i < length; ++i) {
        image.data[i] = cv::saturate_cast<uchar>(contrast * image.data[i] + _brightness);
    }
}
