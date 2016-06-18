#include "Illustrace.h"

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

void Illustrace::binarize()
{
    binarizedImage = sourceImage.clone();
    brightnessFilter.apply(binarizedImage);
    notify(IllustraceEvent::BrightnessFilterApplied);

    blurFilter.apply(binarizedImage);
    notify(IllustraceEvent::BlurFilterApplied);

    binaryThresholdFilter.apply(binarizedImage);
    notify(IllustraceEvent::Binarized);
}

void Illustrace::buildCenterLine()
{
    thinnedImage = binarizedImage.clone();
    thinningFilter.apply(thinnedImage);
    notify(IllustraceEvent::Thinned);

    centerLineKeyPoints.clear();
    featureDetector.detect(thinnedImage, centerLineKeyPoints);
    notify(IllustraceEvent::CenterLineKeyPointDetected);

    centerLines.clear();
    centerLineBuilder.build(thinnedImage, centerLines);
    notify(IllustraceEvent::CenterLineBuilt);
}

void Illustrace::approximateCenterLine()
{
    approximatedCenterLines.clear();

    for (auto lines : centerLines) {
        std::vector<cv::Point> approx;
        cv::approxPolyDP(cv::Mat(lines), approx, MAX(0.0, 1.0 - detail) * 0.005 * cv::arcLength(lines, true), false);
        approximatedCenterLines.push_back(approx);
    }

    notify(IllustraceEvent::CenterLineApproximated);
}

void Illustrace::buildBezierizedCenterLine()
{
    bezierizedCenterLines.clear();

    std::vector<BezierVertex<cv::Point2f>> bezierLine;

    for (auto line : approximatedCenterLines) {
        bezierSplineBuilder.build(line, bezierLine);
        bezierizedCenterLines.push_back(bezierLine);
        bezierLine = std::vector<BezierVertex<cv::Point2f>>();
    }

    notify(IllustraceEvent::CenterLineBezierized);
}

void Illustrace::buildOutline()
{
    negativeImage = binarizedImage.clone();
    negativeFilter.apply(negativeImage);
    notify(IllustraceEvent::NegativeFilterApplied);

    cv::findContours(negativeImage, outlineContours, outlineHierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    notify(IllustraceEvent::OutlineBuilt);
}

void Illustrace::approximateOutline()
{
    approximatedOutlineContours.clear();

    for (auto lines : outlineContours) {
        std::vector<cv::Point> approx;
        cv::approxPolyDP(cv::Mat(lines), approx, MAX(0.0, 1.0 - detail) * 0.005 * cv::arcLength(lines, true), false);
        approximatedOutlineContours.push_back(approx);
    }

    notify(IllustraceEvent::OutlineApproximated);
}
