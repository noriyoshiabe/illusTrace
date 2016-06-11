#pragma once

#include "Observable.h"
#include "BrightnessFilter.h"
#include "BlurFilter.h"
#include "BinaryThresholdFilter.h"
#include "ThinningFilter.h"
#include "EdgeFilter.h"
#include "opencv2/imgproc.hpp"

namespace illustrace {
namespace core {

enum class IllustraceEvent {
    PreviewImageChanged,
};

class Illustrace : public Observable<Illustrace, IllustraceEvent> {
public:
    bool loadSourceImage(const char *filepath);
    void binarize();
    void buildCenterLine();
    void buildOutline();
    void drawKeyPoints(cv::Mat &image, std::vector<cv::KeyPoint> &keyPoints);

    cv::Mat &getPreviewImage();

    BrightnessFilter brightnessFilter;
    BlurFilter blurFilter;
    BinaryThresholdFilter binaryThresholdFilter;
    ThinningFilter thinningFilter;
    EdgeFilter edgeFilter;

    bool plotKeyPoints;

private:
    cv::Mat sourceImage;
    cv::Mat binarizedImage;
    cv::Mat thinnedImage;
    cv::Mat edgeImage;
    cv::Mat previewImage;
};

} // namespace core
} // namespace illustrace
