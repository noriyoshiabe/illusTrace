#pragma once

#include "Observer.h"
#include "Illustrace.h"

#include "opencv2/highgui.hpp"
#include "cairo/cairo.h"

namespace illustrace {

class View : public Observer<Illustrace> {
public:
    View();
    ~View();

    int wait;
    bool step;
    bool plot;

    void waitKey();
    void waitKeyIfNeeded();

private:
    cv::Mat preview;
    cairo_surface_t *surface;
    cairo_t *cr;

    void notify(Illustrace *sender, va_list argList);

    void clearPreview();
    void copyFrom(cv::Mat &image);
    template <class T>
    void drawLines(std::vector<std::vector<T>> &lines, double thickness, bool closePath = false);
    void drawPaths(std::vector<Path *> *paths, double thickness, bool closePath = false);
    template <class T>
    void plotPoints(std::vector<T> &points);
    template <class T>
    void plotPoints(std::vector<std::vector<T>> &lines);
    void plotGraph(Graph &graph);
    void plotPathHandle(std::vector<Path *> *paths);
};

} // namespace illustrace
