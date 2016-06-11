#include "BinaryThresholdFilter.h" 

using namespace illustrace;
using namespace core;

void BinaryThresholdFilter::apply(cv::Mat &image)
{
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}
