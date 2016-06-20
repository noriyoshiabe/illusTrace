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
    int hierarchyDepth;

    void notify(Illustrace *sender, va_list argList);

    void clearPreview();
    void copyFrom(cv::Mat &image);
    template <class T>
    void drawLines(std::vector<std::vector<T>> &lines, double thickness, bool closePath = false);
    void drawBezierLine(std::vector<BezierVertex<cv::Point2f>> &bezierLine, double thickness, bool withPlot = false);
    void drawBezierLines(std::vector<std::vector<BezierVertex<cv::Point2f>>> &bezierLines, double thickness, bool withPlot = false);
    void drawBezierLineContours(std::vector<std::vector<BezierVertex<cv::Point2f>>> &contours, std::vector<cv::Vec4i> &hierarchy, double thickness);
    void drawBezierLineContours2(std::vector<std::vector<BezierVertex<cv::Point2f>>> &contours, std::vector<cv::Vec4i> &hierarchy, int index);
    void fillBezierLineContour(std::vector<BezierVertex<cv::Point2f>> &contour);
    template <class T>
    void plotPoints(std::vector<T> &points);
    template <class T>
    void plotPoints(std::vector<std::vector<T>> &lines);
    void plotGraph(Graph &graph);
    void plotBezierHandle(std::vector<std::vector<BezierVertex<cv::Point2f>>> &bezierLines);
};

} // namespace illustrace
