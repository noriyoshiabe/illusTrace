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

    notify(IllustraceEvent::OnLoadSourceImage, &sourceImage);
    
    return true;
}
