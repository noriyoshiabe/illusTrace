#include "Illustrace.h"

#include <iostream>
#include "opencv2/highgui.hpp"

using namespace illustrace;
using namespace core;

bool Illustrace::loadSourceImage(const char *filename)
{
    sourceImage = imread(filename, cv::IMREAD_GRAYSCALE);
    if (!sourceImage.data) {
        return false;
    }
    
    setResultImage(sourceImage);
    return true;
}

void Illustrace::setResultImage(cv::Mat &image)
{
    resultImage = image;
    
    // TODO notify
}
