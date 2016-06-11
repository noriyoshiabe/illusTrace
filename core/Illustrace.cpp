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

    previewImage = sourceImage;
    notify(IllustraceEvent::PreviewImageChanged);
    return true;
}

void Illustrace::binarize()
{
    binarizedImage = sourceImage.clone();
    brightnessFilter.apply(binarizedImage);
    previewImage = binarizedImage;
    notify(IllustraceEvent::PreviewImageChanged);

    blurFilter.apply(binarizedImage);
    notify(IllustraceEvent::PreviewImageChanged);

    binaryThresholdFilter.apply(binarizedImage);
    notify(IllustraceEvent::PreviewImageChanged);

    thinnedImage = binarizedImage.clone();
    thinningFilter.apply(thinnedImage);
    previewImage = thinnedImage;
    notify(IllustraceEvent::PreviewImageChanged);
}

cv::Mat &Illustrace::getPreviewImage()
{
    return previewImage;
}
