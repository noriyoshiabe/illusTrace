#include "Illustrace.h"

using namespace illustrace;
using namespace core;

Illustrace::Illustrace() : brightness(0.0), blur(5), detail(1.0), thickness(1.0)
{
}

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
    Filter::brightness(binarizedImage, brightness);
    notify(IllustraceEvent::BrightnessFilterApplied);

    Filter::blur(binarizedImage, blur);
    notify(IllustraceEvent::BlurFilterApplied);

    Filter::threshold(binarizedImage);
    notify(IllustraceEvent::Binarized);

    negativeImage = binarizedImage.clone();
    Filter::negative(negativeImage);
    notify(IllustraceEvent::NegativeFilterApplied);

    boundingRect = cv::boundingRect(negativeImage);
}

void Illustrace::buildCenterLine()
{
    thinnedImage = binarizedImage.clone();
    Filter::thinning(thinnedImage);
    notify(IllustraceEvent::Thinned);

    centerLineKeyPoints.clear();
    featureDetector.detect(thinnedImage, centerLineKeyPoints);
    notify(IllustraceEvent::CenterLineKeyPointDetected);

    graphBuilder.build(thinnedImage, centerLineKeyPoints, centerLineGraph);
    notify(IllustraceEvent::CenterLineGraphBuilt);

    graphBuilder.approximate(centerLineGraph, approximatedCenterLineGraph);
    notify(IllustraceEvent::CenterLineGraphApproximated);

    centerLines.clear();
    centerLineBuilder.build(approximatedCenterLineGraph, centerLines);
    notify(IllustraceEvent::CenterLineBuilt);
}

void Illustrace::approximateCenterLine()
{
    approximatedCenterLines.clear();

    for (auto line : centerLines) {
        std::vector<cv::Point2f> approx;
        bool needAdjust = line.front() == line.back();
        cv::approxPolyDP(cv::Mat(line), approx, epsilon(), false);
        if (needAdjust && approx.front() != approx.back()) {
            approx.push_back(approx.front());
        }
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
    cv::findContours(negativeImage, outlineContours, outlineHierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    notify(IllustraceEvent::OutlineBuilt);
}

void Illustrace::approximateOutline()
{
    approximatedOutlineContours.clear();

    for (auto line : outlineContours) {
        std::vector<cv::Point2f> approx;
        cv::approxPolyDP(cv::Mat(line), approx, epsilon(), false);
        approximatedOutlineContours.push_back(approx);
    }

    notify(IllustraceEvent::OutlineApproximated);
}

void Illustrace::buildBezierizedOutline()
{
    bezierizedOutlineContours.clear();

    std::vector<BezierVertex<cv::Point2f>> bezierLine;

    for (auto line : approximatedOutlineContours) {
        bezierSplineBuilder.build(line, bezierLine, true);
        bezierizedOutlineContours.push_back(bezierLine);
        bezierLine = std::vector<BezierVertex<cv::Point2f>>();
    }

    notify(IllustraceEvent::OutlineBezierized);
}

double Illustrace::epsilon()
{
    double shortSide = MIN(boundingRect.width, boundingRect.height);
    return shortSide * (0.006250 / detail); // Inverse proportion. 0.5 to 1.25%
}
