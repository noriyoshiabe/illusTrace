#pragma once

#include "Observer.h"
#include "Illustrace.h"

#include "opencv2/highgui.hpp"
#include "cairo/cairo.h"

namespace illustrace {
namespace cli {

class View : public core::Observer<core::Illustrace, core::IllustraceEvent> {
public:
    View();
    ~View();

    int wait;
    bool step;

    void waitKey();

private:
    cv::Mat preview;
    cairo_surface_t *surface;
    cairo_t *cr;

    void notify(core::Illustrace *sender, core::IllustraceEvent event, va_list argList);

    void drawLines(std::vector<std::vector<cv::Point>> lines, double thickness);
    void drawBezierLines(std::vector<std::vector<core::BezierVertex<cv::Point2f>>> bezierLines, double thickness);
};

} // namespace cli
} // namespace illustrace
