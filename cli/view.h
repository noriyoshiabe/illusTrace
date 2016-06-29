#pragma once

#include "Observer.h"
#include "Illustrace.h"
#include "Editor.h"

#include "opencv2/highgui.hpp"
#include "cairo/cairo.h"

namespace illustrace {

class View : public Observer<Illustrace>, public Observer<Editor>, public Observer<Document> {
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
    void notify(Editor *sender, va_list argList);
    void notify(Document *sender, va_list argList);

    void clearPreview();
    void fillBackground(cv::Scalar &color);
    void copyFrom(cv::Mat &image);
    template <class T>
    void drawLines(std::vector<std::vector<T>> &lines, double thickness, bool closePath = false);
    void drawPaths(std::vector<Path *> *paths, double thickness, cv::Scalar &stroke, cv::Scalar &fill);
    void drawPath(Path *path, double thickness);
    template <class T>
    void plotPoints(std::vector<T> &points);
    template <class T>
    void plotPoints(std::vector<std::vector<T>> &lines);
    void plotGraph(Graph &graph);
    void plotPathsHandle(std::vector<Path *> *paths);
    void plotPathHandle(Path *path);
};

} // namespace illustrace
