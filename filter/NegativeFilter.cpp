#include "NegativeFilter.h" 

using namespace illustrace;
using namespace filter;

void NegativeFilter::apply(cv::Mat &image)
{
    int length = image.rows * image.cols;
    for (int i = 0; i < length; ++i) {
        image.data[i] = 255 - image.data[i];
    }
}
