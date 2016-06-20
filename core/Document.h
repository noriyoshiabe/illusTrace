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

enum class DocumentEvent {
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

class Document : public Observable<Document, DocumentEvent> {
public:
    Document();
    ~Document();

    LineMode mode();
    double brightness();
    double detail();
    double thickness();
    double scale();
    double rotation();
    cv::Mat &correctionLayer();
    cv::Rect &clippingRect();
    cv::Mat &sourceImage();
    cv::Mat &binarizedImage();
    cv::Rect &boundingRect();
    std::vector<Path *> *paths();

    void mode(LineMode mode);
    void brightness(double brightness);
    void thickness(double thickness);
    void scale(double scale);
    void rotation(double rotation);
    void correctionLayer(cv::Mat &layer);
    void clippingRect(cv::Rect &rect);
    void sourceImage(cv::Mat &sourceImage);
    void binarizedImage(cv::Mat &binarizedImage);
    void boundingRect(cv::Rect &rect);
    void path(std::vector<Path *> *path);

private:
    double _brightness;
    double _detail;
    double _thickness;
    double _scale;
    double _rotation;
    cv::Mat _correctionLayer;
    cv::Rect _clippingRect;

    cv::Mat _sourceImage;
    cv::Mat _binarizedImage;
    cv::Rect _boundingRect;
    std::vector<Path *> *_paths;
};

} // namespace illustrace
