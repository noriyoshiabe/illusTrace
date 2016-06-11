#include "EdgeFilter.h" 

using namespace illustrace;
using namespace core;

void EdgeFilter::apply(cv::Mat &image)
{
    cv::Canny(image, image, 100, 200);

    int length = image.rows * image.cols;
    for (int i = 0; i < length; ++i) {
        image.data[i] = 255 - image.data[i];
    }
}
