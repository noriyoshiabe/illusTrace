#include "BrightnessFilter.h" 

using namespace illustrace;
using namespace filter;

void BrightnessFilter::apply(cv::Mat &image)
{
    double _brightness = static_cast<double>(brightness * 255.0);

    int length = image.rows * image.cols;
    for (int i = 0; i < length; ++i) {
        image.data[i] = cv::saturate_cast<uchar>(contrast * image.data[i] + _brightness);
    }
}
