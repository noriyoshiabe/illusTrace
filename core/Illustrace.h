#pragma once

#include "Observable.h"
#include "BrightnessFilter.h"
#include "opencv2/imgproc.hpp"

namespace illustrace {
namespace core {

enum class IllustraceEvent {
    SourceImageLoaded,
    BrightnessChanged,
};

class Illustrace : public Observable<Illustrace, IllustraceEvent> {
public:
    bool loadSourceImage(const char *filepath);
    void adjustBrightness(double brightness, double contrast = 1.0);

    cv::Mat &getSourceImage();
    cv::Mat &getBrightnessChangedImage();

private:
    cv::Mat sourceImage;
    cv::Mat brightnessChangedImage;

    BrightnessFilter brightnessFilter;
};

} // namespace core
} // namespace illustrace
