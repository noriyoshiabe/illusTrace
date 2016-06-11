#pragma once

#include "Observable.h"
#include "BrightnessFilter.h"
#include "opencv2/imgproc.hpp"

namespace illustrace {
namespace core {

enum class IllustraceEvent {
    PreviewImageChanged,
};

class Illustrace : public Observable<Illustrace, IllustraceEvent> {
public:
    bool loadSourceImage(const char *filepath);
    void binarize(double brightness);

    cv::Mat &getPreviewImage();

private:
    cv::Mat sourceImage;
    cv::Mat binarizedImage;
    cv::Mat previewImage;

    BrightnessFilter brightnessFilter;
};

} // namespace core
} // namespace illustrace
