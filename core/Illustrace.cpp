#include "Illustrace.h"

#include <iostream>
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"

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
}

void Illustrace::buildCenterLine()
{
    thinnedImage = binarizedImage.clone();
    thinningFilter.apply(thinnedImage);
    previewImage = thinnedImage;
    notify(IllustraceEvent::PreviewImageChanged);

    std::vector<cv::KeyPoint> keyPoints;

    cv::Ptr<cv::GFTTDetector> detector = cv::GFTTDetector::create(0, 0.01, 4);
    detector->detect(thinnedImage, keyPoints);

    if (plotKeyPoints) {
        drawKeyPoints(thinnedImage, keyPoints);
    }
}

void Illustrace::buildOutline()
{
    edgeImage = binarizedImage.clone();
    edgeFilter.apply(edgeImage);
    previewImage = edgeImage;
    notify(IllustraceEvent::PreviewImageChanged);

    std::vector<cv::KeyPoint> keyPoints;

    cv::Ptr<cv::GFTTDetector> detector = cv::GFTTDetector::create(0, 0.01, 4);
    detector->detect(edgeImage, keyPoints);

    if (plotKeyPoints) {
        drawKeyPoints(edgeImage, keyPoints);
    }
}

void Illustrace::drawKeyPoints(cv::Mat &image, std::vector<cv::KeyPoint> &keyPoints)
{
    cv::Mat dstImage;
    cv::drawKeypoints(image, keyPoints, dstImage);
    previewImage = dstImage;
    notify(IllustraceEvent::PreviewImageChanged);
}

cv::Mat &Illustrace::getPreviewImage()
{
    return previewImage;
}
