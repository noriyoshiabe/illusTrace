#include "BlurFilter.h" 

using namespace illustrace;
using namespace core;

void BlurFilter::apply(cv::Mat &image)
{
    cv::GaussianBlur(image, image, cv::Size(blur, blur), 0, 0);
}
