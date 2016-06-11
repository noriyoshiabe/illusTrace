#include "Illustrace.h"

#include <iostream>
#include "opencv2/highgui.hpp"

using namespace illustrace;
using namespace core;

bool Illustrace::loadSourceImage(const char *filepath)
{
    sourceImage = imread(filepath, cv::IMREAD_GRAYSCALE);
    if (!sourceImage.data) {
        return false;
    }

    notify(IllustraceEvent::SourceImageLoaded);
    
    return true;
}
void Illustrace::adjustBrightness(double brightness, double contrast)
{
    brightnessFilter.brightness = brightness;
    brightnessFilter.contrast = contrast;
    brightnessChangedImage = brightnessFilter.apply(sourceImage);

    notify(IllustraceEvent::BrightnessChanged);
}

cv::Mat &Illustrace::getSourceImage()
{
    return sourceImage;
}

cv::Mat &Illustrace::getBrightnessChangedImage()
{
    return brightnessChangedImage;
}
