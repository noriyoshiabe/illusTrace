#pragma once

#include "Observable.h"
#include "opencv2/imgproc.hpp"

namespace illustrace {
namespace core {

enum class IllustraceEvent {
    OnLoadSourceImage,
};

class Illustrace : public Observable<Illustrace, IllustraceEvent> {
public:
    bool loadSourceImage(const char *filename);

private:
    cv::Mat sourceImage;
};

} // namespace core
} // namespace illustrace
