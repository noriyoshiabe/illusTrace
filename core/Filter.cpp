#include "Filter.h"

using namespace illustrace;

void Filter::brightness(cv::Mat &image, double brightness, double contrast)
{
    brightness *= 255.0;

    uchar *data = image.data;
    size_t step = image.step[0];
    
    for (int j = 0; j < image.rows; ++j) {
        data += step;
        
        for (int i = 0; i < step; ++i) {
            data[i] = cv::saturate_cast<uchar>(contrast * data[i] + brightness);
        }
    }
}

void Filter::brightnessBGRA(cv::Mat &image, double brightness, double contrast)
{
    brightness *= 255.0;

    uchar *data = image.data;
    size_t step = image.step[0];
    
    for (int j = 0; j < image.rows; ++j) {
        data += step;
        
        for (int i = 0; i < step; i += 4) {
            data[i+0] = cv::saturate_cast<uchar>(contrast * data[i+0] + brightness);
            data[i+1] = cv::saturate_cast<uchar>(contrast * data[i+1] + brightness);
            data[i+2] = cv::saturate_cast<uchar>(contrast * data[i+2] + brightness);
        }
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
    uchar *data = image.data;
    size_t step = image.step[0];
    
    for (int j = 0; j < image.rows; ++j) {
        data += step;
        
        for (int i = 0; i < step; ++i) {
            data[i] = 255 - data[i];
        }
    }
}
