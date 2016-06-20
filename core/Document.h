#pragma once

#include "Observable.h"

#include "opencv2/opencv.hpp"
#include <vector>

namespace illustrace {

enum class LineMode {
    Center,
    Outline,
};

struct Segment {
    enum class Type {
        Point,
        Line,
        Curve,
    };

    Type type;
    cv::Point2f p1;
    cv::Point2f p2;
    cv::Point2f p3;

    Segment(cv::Point2f p1) {
        this->type = Type::Point;
        this->p1 = p1;
    }

    Segment(cv::Point2f p1, cv::Point2f p2) {
        this->type = Type::Line;
        this->p1 = p1;
        this->p2 = p2;
    }

    Segment(cv::Point2f p1, cv::Point2f p2, cv::Point2f) {
        this->type = Type::Curve;
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
    }
};

struct Path {
    std::vector<Segment> segments;
    bool closed;
    Path *child;

    Path() : closed(false), child(nullptr) {};
};

class Document : public Observable<Document> {
public:
    enum Event : int {
        Mode,
        Brightness,
        Blur,
        Detail,
        Smoothing,
        Thickness,
        Scale,
        Rotation,
        CorrectionLayer,
        ClippingRect,
        BoundingRect,
        Paths,
        SourceImage,
        BinarizedImage,
        NegativeImage,
        CenterLines,
        ApproximatedCenterLines,
        OutlineContours,
        ApproximatedOutlineContours,
        OutlineHierarchy,
    };

    Document();
    ~Document();

    LineMode mode();
    double brightness();
    double blur();
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
    cv::Mat &negativeImage();
    std::vector<std::vector<cv::Point2f>> *centerLines();
    std::vector<std::vector<cv::Point2f>> *approximatedCenterLines();
    std::vector<std::vector<cv::Point>> *outlineContours();
    std::vector<std::vector<cv::Point2f>> *approximatedOutlineContours();
    std::vector<cv::Vec4i> *outlineHierarchy();

    void mode(LineMode mode);
    void brightness(double brightness);
    void blur(double blur);
    void detail(double detail);
    void smoothing(double smoothing);
    void thickness(double thickness);
    void scale(double scale);
    void rotation(double rotation);
    void correctionLayer(cv::Mat &layer);
    void clippingRect(cv::Rect &rect);
    void boundingRect(cv::Rect &rect);
    void paths(std::vector<Path *> *paths);
    void sourceImage(cv::Mat &sourceImage);
    void binarizedImage(cv::Mat &binarizedImage);
    void negativeImage(cv::Mat &negativeImage);
    void centerLines(std::vector<std::vector<cv::Point2f>> *centerLines);
    void approximatedCenterLines(std::vector<std::vector<cv::Point2f>> *approximatedCenterLines);
    void outlineContours(std::vector<std::vector<cv::Point>> *outlineContours);
    void approximatedOutlineContours(std::vector<std::vector<cv::Point2f>> *approximatedOutlineContours);
    void outlineHierarchy(std::vector<cv::Vec4i> *outlineHierarchy);

private:
    double _brightness;
    double _blur;
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
    cv::Mat _negativeImage;

    std::vector<std::vector<cv::Point2f>> *_centerLines;
    std::vector<std::vector<cv::Point2f>> *_approximatedCenterLines;
    std::vector<std::vector<cv::Point>> *_outlineContours;
    std::vector<std::vector<cv::Point2f>> *_approximatedOutlineContours;
    std::vector<cv::Vec4i> *_outlineHierarchy;
};

} // namespace illustrace
