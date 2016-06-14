#pragma once

#include "Observable.h"
#include "BrightnessFilter.h"
#include "BlurFilter.h"
#include "BinaryThresholdFilter.h"
#include "ThinningFilter.h"
#include "NegativeFilter.h"
#include "CenterLineBuilder.h"
#include "BezierSplineBuilder.h"
#include "opencv2/imgproc.hpp"

namespace illustrace {
namespace core {

enum class IllustraceEvent {
    PreviewImageChanged,
};

class Illustrace : public Observable<Illustrace, IllustraceEvent> {
public:
    Illustrace() : detail(1.0), thickness(1) {};

    bool loadSourceImage(const char *filepath);
    void binarize();
    void buildCenterLine();
    void approximateCenterLine();
    void buildOutline();
    void approximateOutline();

    void drawCenterLines(std::vector<std::vector<cv::Point>> lines);
    void drawContours(std::vector<std::vector<cv::Point>> contours, std::vector<cv::Vec4i> hierarchy);
    void drawKeyPoints(cv::Mat &image, std::vector<cv::Point> &keyPoints);

    cv::Mat &getPreviewImage();

    filter::BrightnessFilter brightnessFilter;
    filter::BlurFilter blurFilter;
    filter::BinaryThresholdFilter binaryThresholdFilter;
    filter::ThinningFilter thinningFilter;
    filter::NegativeFilter negativeFilter;

    CenterLineBuilder centerLineBuilder;

    bool plotKeyPoints;
    bool plotLines;
    bool step;
    bool antiAlias;

    std::vector<std::vector<cv::Point>> centerLines;
    std::vector<std::vector<cv::Point>> approximatedCenterLines;

    std::vector<std::vector<cv::Point>> outlineContours;
    std::vector<std::vector<cv::Point>> approximatedOutlineContours;
    std::vector<cv::Vec4i> outlineHierarchy;

    double detail;
    int thickness;

private:
    cv::Mat sourceImage;
    cv::Mat binarizedImage;
    cv::Mat previewImage;
};

} // namespace core
} // namespace illustrace
