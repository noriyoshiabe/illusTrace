#pragma once

#include "Observable.h"

#include "opencv2/opencv.hpp"
#include <vector>

namespace illustrace {

enum class LineMode {
    Center,
    Outline,
};

enum class SegmentType {
    Point,
    Line,
    Curve,
};

struct Segment {
    SegmentType type;
    cv::Point2f p[3];
};

struct Path {
    std::vector<Segment> segments;
    bool closed;
    Path *child;
};

class Document : public Observable<Document> {
public:
    enum Event : int {
        Brightness,
        Detail,
        Thickness,
        Scale,
        Rotation,
        CorrectionLayer,
        ClippingRect,
        SourceImage,
        BinarizedImage,
        BoundingRect,
        Paths,
    };

    Document();
    ~Document();

    LineMode mode();
    double brightness();
    double detail();
    double smoothing();
    double thickness();
    double scale();
    double rotation();
    cv::Mat &correctionLayer();
    cv::Rect &clippingRect();
    cv::Rect &boundingRect();
    std::vector<Path *> *paths();
    cv::Mat &sourceImage();
    cv::Mat &binarizedImage();
    std::vector<std::vector<cv::Point2f>> &centerLines();
    std::vector<std::vector<cv::Point>> &outlineContours();
    std::vector<cv::Vec4i> &outlineHierarchy();

    void mode(LineMode mode);
    void brightness(double brightness);
    void detail(double detail);
    void smoothing(double detail);
    void thickness(double thickness);
    void scale(double scale);
    void rotation(double rotation);
    void correctionLayer(cv::Mat &layer);
    void clippingRect(cv::Rect &rect);
    void boundingRect(cv::Rect &rect);
    void path(std::vector<Path *> *path);
    void sourceImage(cv::Mat &sourceImage);
    void binarizedImage(cv::Mat &binarizedImage);
    void centerLines(std::vector<std::vector<cv::Point2f>> &centerLines);
    void outlineContours(std::vector<std::vector<cv::Point>> &outlineContours);
    void outlineHierarchy(std::vector<cv::Vec4i> &outlineHierarchy);

private:
    double _brightness;
    double _detail;
    double _smoothing;
    double _thickness;
    double _scale;
    double _rotation;
    cv::Mat _correctionLayer;
    cv::Rect _clippingRect;
    cv::Rect _boundingRect;
    std::vector<Path *> *_paths;

    cv::Mat _sourceImage;
    cv::Mat _binarizedImage;

    std::vector<std::vector<cv::Point2f>> _centerLines;
    std::vector<std::vector<cv::Point>> _outlineContours;
    std::vector<cv::Vec4i> _outlineHierarchy;
};

} // namespace illustrace
