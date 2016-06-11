#pragma once

#include "Observable.h"
#include "BrightnessFilter.h"
#include "BlurFilter.h"
#include "BinaryThresholdFilter.h"
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

    cv::Mat &getPreviewImage();

    BrightnessFilter brightnessFilter;
    BlurFilter blurFilter;
    BinaryThresholdFilter binaryThresholdFilter;

private:
    cv::Mat sourceImage;
    cv::Mat binarizedImage;
    cv::Mat previewImage;
};

} // namespace core
} // namespace illustrace
