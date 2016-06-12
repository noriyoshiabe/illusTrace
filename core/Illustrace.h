#pragma once

#include "Observable.h"
#include "BrightnessFilter.h"
#include "BlurFilter.h"
#include "BinaryThresholdFilter.h"
#include "ThinningFilter.h"
#include "EdgeFilter.h"
#include "CenterLineBuilder.h"
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

    void drawCenterLines(std::vector<std::vector<cv::Point>> lines);
    void drawKeyPoints(cv::Mat &image, std::vector<cv::KeyPoint> &keyPoints);

    cv::Mat &getPreviewImage();

    filter::BrightnessFilter brightnessFilter;
    filter::BlurFilter blurFilter;
    filter::BinaryThresholdFilter binaryThresholdFilter;
    filter::ThinningFilter thinningFilter;
    filter::EdgeFilter edgeFilter;

    CenterLineBuilder centerLineBuilder;

    bool plotKeyPoints;
    bool plotLines;

    std::vector<std::vector<cv::Point>> centerLines;
    std::vector<std::vector<cv::Point>> approximatedCenterLines;

    double detail;
    int thickness;

private:
    cv::Mat sourceImage;
    cv::Mat binarizedImage;
    cv::Mat edgeImage;
    cv::Mat previewImage;
};

} // namespace core
} // namespace illustrace
