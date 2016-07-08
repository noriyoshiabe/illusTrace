#include "Filter.h"

using namespace illustrace;

void Filter::brightness(cv::Mat &image, double brightness, double contrast)
{
    brightness *= 255.0;

    int length = image.rows * image.cols;
    for (int i = 0; i < length; ++i) {
        image.data[i] = cv::saturate_cast<uchar>(contrast * image.data[i] + brightness);
    }
}

void Filter::brightnessBGRA(cv::Mat &image, double brightness, double contrast)
{
    brightness *= 255.0;

    int length = image.rows * image.cols * 4;
    for (int i = 0; i < length; i += 4) {
        image.data[i+0] = cv::saturate_cast<uchar>(contrast * image.data[i+0] + brightness);
        image.data[i+1] = cv::saturate_cast<uchar>(contrast * image.data[i+1] + brightness);
        image.data[i+2] = cv::saturate_cast<uchar>(contrast * image.data[i+2] + brightness);
    }
}

void Filter::blur(cv::Mat &image, int blur)
{
    cv::GaussianBlur(image, image, cv::Size(blur, blur), 0, 0);
}

void Filter::threshold(cv::Mat &image)
{
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}

void Filter::negative(cv::Mat &image)
{
    int length = image.rows * image.cols;
    for (int i = 0; i < length; ++i) {
        image.data[i] = 255 - image.data[i];
    }
}
