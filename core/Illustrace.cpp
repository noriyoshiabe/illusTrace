#include "Illustrace.h"
#include "BezierPath.h"

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
}

void Illustrace::buildCenterLine()
{
    cv::Mat image = binarizedImage.clone();
    thinningFilter.apply(image);
    previewImage = image;
    notify(IllustraceEvent::PreviewImageChanged);

    centerLines.clear();
    centerLineBuilder.build(image, centerLines);

    if (plotLines) {
        drawCenterLines(centerLines);
    }
}

void Illustrace::approximateCenterLine()
{
    approximatedCenterLines.clear();

    for (auto lines : centerLines) {
        std::vector<cv::Point> approx;
        cv::approxPolyDP(cv::Mat(lines), approx, MAX(0.0, 1.0 - detail) * 0.005 * cv::arcLength(lines, true), false);
        approximatedCenterLines.push_back(approx);
    }

    if (plotLines) {
        drawCenterLines(approximatedCenterLines);
    }
}

void Illustrace::buildBezierizedCenterLine()
{
    bezierizedCenterLine.clear();

    std::vector<BezierVertex<cv::Point2f>> bezierLine;

    for (auto line : approximatedCenterLines) {
        bezierSplineBuilder.build(line, bezierLine);
        bezierizedCenterLine.push_back(bezierLine);
        bezierLine = std::vector<BezierVertex<cv::Point2f>>();
    }

    if (plotLines) {
        drawBezierizedLine(bezierizedCenterLine);
    }
}

void Illustrace::buildOutline()
{
    cv::Mat negative = binarizedImage.clone();
    negativeFilter.apply(negative);

    cv::findContours(negative, outlineContours, outlineHierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    if (plotLines) {
        drawContours(outlineContours, outlineHierarchy);
    }
}

void Illustrace::approximateOutline()
{
    approximatedOutlineContours.clear();

    for (auto lines : outlineContours) {
        std::vector<cv::Point> approx;
        cv::approxPolyDP(cv::Mat(lines), approx, MAX(0.0, 1.0 - detail) * 0.005 * cv::arcLength(lines, true), false);
        approximatedOutlineContours.push_back(approx);
    }

    if (plotLines) {
        drawContours(approximatedOutlineContours, outlineHierarchy);
    }
}

void Illustrace::drawCenterLines(std::vector<std::vector<cv::Point>> lines)
{
    int lineType = antiAlias ? CV_AA : 8;

    previewImage.setTo(cv::Scalar(255, 255, 255));

    for (auto line : lines) {
        int sizeMinus1 = line.size() - 1;
        if (0 == sizeMinus1) {
            cv::line(previewImage, line[0], line[0], cv::Scalar(0), thickness, lineType);
        }
        else {
            for (int i = 0; i < sizeMinus1; ++i) {
                cv::line(previewImage, line[i], line[i + 1], cv::Scalar(0), thickness, lineType);
            }
        }

        if (step) {
            notify(IllustraceEvent::PreviewImageChanged);
        }
    }

    notify(IllustraceEvent::PreviewImageChanged);
}

void Illustrace::drawContours(std::vector<std::vector<cv::Point>> contours, std::vector<cv::Vec4i> hierarchy)
{
    int lineType = antiAlias ? CV_AA : 8;

    previewImage.setTo(cv::Scalar(255, 255, 255));
    
    for(int idx = 0; 0 <= idx; idx = hierarchy[idx][0]) {
        cv::drawContours(previewImage, contours, idx, cv::Scalar(0), CV_FILLED, lineType, hierarchy);
    }

    notify(IllustraceEvent::PreviewImageChanged);
}

void Illustrace::drawBezierizedLine(std::vector<std::vector<BezierVertex<cv::Point2f>>> bezierLines)
{
    previewImage = cv::Mat(previewImage.rows, previewImage.cols, CV_8UC4);
    previewImage.setTo(cv::Scalar(255, 255, 255));

#if 0
    for (auto b : bezierLines) {
        int sizeMinus1 = b.size() - 1;
        for (int i = 0; i < sizeMinus1; ++i) {
            cv::line(previewImage, b[i].pt, b[i + 1].pt, cv::Scalar(0), thickness, 8);
            //cv::line(previewImage, b[i].ctl.prev, b[i + 1].ctl.prev, cv::Scalar(0), thickness, 8);
            //cv::line(previewImage, b[i].ctl.next, b[i + 1].ctl.next, cv::Scalar(0), thickness, 8);
        }
    }

    notify(IllustraceEvent::PreviewImageChanged);
    return;
#endif

    BezierPath bezierPath = BezierPath(previewImage, cv::Scalar(0, 0, 0), thickness);

    for (auto bezierLine : bezierLines) {
        bezierPath.moveToPoint(bezierLine[0].pt);
        cv::Point2f ctl1 = bezierLine[0].ctl.next;

        int length = bezierLine.size();
        for (int i = 1; i < length; ++i) {
            BezierVertex<cv::Point2f> vtx = bezierLine[i];
            bezierPath.curveToPoint(vtx.pt, ctl1, vtx.ctl.prev);
            //bezierPath.curveToPoint(vtx.pt, vtx.pt, vtx.pt);
            ctl1 = vtx.ctl.next;
        }

        bezierPath.stroke();
        //bezierPath.plotContolPointAndHandles(previewImage);
        notify(IllustraceEvent::PreviewImageChanged);
        //cv::waitKey(0);
    }

    notify(IllustraceEvent::PreviewImageChanged);
}

cv::Mat &Illustrace::getPreviewImage()
{
    return previewImage;
}
