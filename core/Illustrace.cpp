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
    cv::Mat image = binarizedImage.clone();
    thinningFilter.apply(image);
    previewImage = image;
    notify(IllustraceEvent::PreviewImageChanged);

    std::vector<cv::Point> keyPoints;
    featureDetector.detect(image, keyPoints);

    if (plotKeyPoints) {
        drawKeyPoints(image, keyPoints);
    }

    centerLines.clear();
    centerLineBuilder.build(image, keyPoints, centerLines);

    if (plotLines) {
        drawCenterLines(centerLines);
    }
}

void Illustrace::approximateCenterLine()
{
    approximatedCenterLines.empty();

    for (auto lines : centerLines) {
        std::vector<cv::Point> approx;
        cv::approxPolyDP(cv::Mat(lines), approx, MAX(0.0, 1.0 - detail) * 0.005 * cv::arcLength(lines, true), false);
        approximatedCenterLines.push_back(approx);
    }

    if (plotLines) {
        drawCenterLines(approximatedCenterLines);
    }
}

void Illustrace::drawCenterLines(std::vector<std::vector<cv::Point>> lines)
{
    previewImage.setTo(cv::Scalar(255, 255, 255));

    for (auto line : lines) {
        int sizeMinus1 = line.size() - 1;
        if (0 == sizeMinus1) {
            cv::line(previewImage, line[0], line[0], cv::Scalar(0), thickness);
        }
        else {
            for (int i = 0; i < sizeMinus1; ++i) {
                cv::line(previewImage, line[i], line[i + 1], cv::Scalar(0), thickness);
            }
        }

        notify(IllustraceEvent::PreviewImageChanged);
    }
}

void Illustrace::buildOutline()
{
    edgeImage = binarizedImage.clone();
    edgeFilter.apply(edgeImage);
    previewImage = edgeImage;
    notify(IllustraceEvent::PreviewImageChanged);

    std::vector<cv::Point> keyPoints;
    featureDetector.detect(edgeImage, keyPoints);

    if (plotKeyPoints) {
        drawKeyPoints(edgeImage, keyPoints);
    }
}

void Illustrace::drawKeyPoints(cv::Mat &image, std::vector<cv::Point> &keyPoints)
{
    cv::Mat dstImage;
    cv::cvtColor(image, dstImage, CV_GRAY2RGB);
    for (auto point : keyPoints) {
        cv::circle(dstImage, point, 5, cv::Scalar(0, 0, 255));
    }

    previewImage = dstImage;
    notify(IllustraceEvent::PreviewImageChanged);
}

cv::Mat &Illustrace::getPreviewImage()
{
    return previewImage;
}
