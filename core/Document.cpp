#include "Document.h"

using namespace illustrace;

Document::Document() :
    _mode(LineMode::Center),
    _brightness(0.0),
    _blur(1.0),
    _detail(1.0),
    _smoothing(1.0),
    _thickness(1.0),
    _transform(cv::Mat_<double>(2, 3)),
    _color(cv::Scalar(0, 0, 0)),
    _backgroundColor(cv::Scalar(255, 255, 255)),
    _backgroundEnable(false),
    _paths(nullptr),
    _paintPaths(nullptr),
    _centerLines(nullptr),
    _approximatedCenterLines(nullptr),
    _outlineContours(nullptr),
    _approximatedOutlineContours(nullptr),
    _outlineHierarchy(nullptr)
{
}

Document::~Document()
{
    if (_paths) {
        for (auto *path : *_paths) {
            delete path;
        }
        delete _paths;
    }

    if (_paintPaths) {
        for (auto *path : *_paintPaths) {
            delete path;
        }
        delete _paintPaths;
    }

    if (_centerLines) {
        delete _centerLines;
    }

    if (_approximatedCenterLines) {
        delete _approximatedCenterLines;
    }

    if (_outlineContours) {
        delete _outlineContours;
    }

    if (_approximatedOutlineContours) {
        delete _approximatedOutlineContours;
    }

    if (_outlineHierarchy) {
        delete _outlineHierarchy;
    }
}

LineMode Document::mode()
{
    return _mode;
}

double Document::brightness()
{
    return _brightness;
}

double Document::blur()
{
    return _blur;
}

double Document::detail()
{
    return _detail;
}

double Document::smoothing()
{
    return _smoothing;
}

double Document::thickness()
{
    return _thickness;
}

cv::Mat &Document::transform()
{
    return _transform;
}

cv::Scalar &Document::color()
{
    return _color;
}

cv::Scalar &Document::backgroundColor()
{
    return _backgroundColor;
}

bool Document::backgroundEnable()
{
    return _backgroundEnable;
}

cv::Mat &Document::paintLayer()
{
    return _paintLayer;
}

cv::Mat &Document::paintMask()
{
    return _paintMask;
}

cv::Rect &Document::clippingRect()
{
    return _clippingRect;
}

cv::Rect &Document::boundingRect()
{
    return _boundingRect;
}

std::vector<Path *> *Document::paths()
{
    return _paths;
}

std::vector<Path *> *Document::paintPaths()
{
    return _paintPaths;
}

cv::Mat &Document::binarizedImage()
{
    return _binarizedImage;
}

cv::Mat &Document::preprocessedImage()
{
    return _preprocessedImage;
}

std::vector<std::vector<cv::Point2f>> *Document::centerLines()
{
    return _centerLines;
}

std::vector<std::vector<cv::Point2f>> *Document::approximatedCenterLines()
{
    return _approximatedCenterLines;
}

std::vector<std::vector<cv::Point>> *Document::outlineContours()
{
    return _outlineContours;
}

std::vector<std::vector<cv::Point2f>> *Document::approximatedOutlineContours()
{
    return _approximatedOutlineContours;
}

std::vector<cv::Vec4i> *Document::outlineHierarchy()
{
    return _outlineHierarchy;
}

void Document::mode(LineMode mode)
{
    _mode = mode;
    notify(this, Document::Event::Mode);
}

void Document::brightness(double brightness)
{
    _brightness = brightness;
    notify(this, Document::Event::Brightness);
}

void Document::blur(double blur)
{
    _blur = blur;
    notify(this, Document::Event::Blur);
}

void Document::detail(double detail)
{
    _detail = detail;
    notify(this, Document::Event::Detail);
}

void Document::smoothing(double smoothing)
{
    _smoothing = smoothing;
    notify(this, Document::Event::Smoothing);
}

void Document::thickness(double thickness)
{
    _thickness = thickness;
    notify(this, Document::Event::Thickness);
}

void Document::transform(cv::Mat &transform)
{
    _transform = transform;
    notify(this, Document::Event::Transform);
}

void Document::color(cv::Scalar &color)
{
    _color = color;
    notify(this, Document::Event::Color);
}

void Document::backgroundColor(cv::Scalar &backgroundColor)
{
    _backgroundColor = backgroundColor;
    notify(this, Document::Event::BackgroundColor);
}

void Document::backgroundEnable(bool backgroundEnable)
{
    _backgroundEnable = backgroundEnable;
    notify(this, Document::Event::BackgroundEnable);
}

void Document::paintLayer(cv::Mat &paintLayer)
{
    _paintLayer = paintLayer;
    notify(this, Document::Event::PaintLayer);
}

void Document::paintMask(cv::Mat &paintMask)
{
    _paintMask = paintMask;
    notify(this, Document::Event::PaintMask);
}

void Document::clippingRect(cv::Rect &rect)
{
    _clippingRect = rect;
    notify(this, Document::Event::ClippingRect);
}

void Document::boundingRect(cv::Rect &rect)
{
    _boundingRect = rect;
    notify(this, Document::Event::BoundingRect);
}

void Document::paths(std::vector<Path *> *paths)
{
    if (_paths) {
        for (auto *path : *_paths) {
            delete path;
        }
        delete _paths;
    }
    _paths = paths;
    notify(this, Document::Event::Paths);
}

void Document::paintPaths(std::vector<Path *> *paintPaths)
{
    if (_paintPaths) {
        for (auto *path : *_paintPaths) {
            delete path;
        }
        delete _paintPaths;
    }
    _paintPaths = paintPaths;
    notify(this, Document::Event::PaintPaths);
}

void Document::binarizedImage(cv::Mat &binarizedImage)
{
    _binarizedImage = binarizedImage;
    notify(this, Document::Event::BinarizedImage);
}

void Document::preprocessedImage(cv::Mat &preprocessedImage)
{
    _preprocessedImage = preprocessedImage;
    notify(this, Document::Event::PreprocessedImage);
}

void Document::centerLines(std::vector<std::vector<cv::Point2f>> *centerLines)
{
    if (_centerLines) {
        delete _centerLines;
    }
    _centerLines = centerLines;
    notify(this, Document::Event::CenterLines);
}

void Document::approximatedCenterLines(std::vector<std::vector<cv::Point2f>> *approximatedCenterLines)
{
    if (_approximatedCenterLines) {
        delete _approximatedCenterLines;
    }
    _approximatedCenterLines = approximatedCenterLines;
    notify(this, Document::Event::ApproximatedCenterLines);
}

void Document::outlineContours(std::vector<std::vector<cv::Point>> *outlineContours)
{
    if (_outlineContours) {
        delete _outlineContours;
    }
    _outlineContours = outlineContours;
    notify(this, Document::Event::OutlineContours);
}

void Document::approximatedOutlineContours(std::vector<std::vector<cv::Point2f>> *approximatedOutlineContours)
{
    if (_approximatedOutlineContours) {
        delete _approximatedOutlineContours;
    }
    _approximatedOutlineContours = approximatedOutlineContours;
    notify(this, Document::Event::ApproximatedOutlineContours);
}

void Document::outlineHierarchy(std::vector<cv::Vec4i> *outlineHierarchy)
{
    if (_outlineHierarchy) {
        delete _outlineHierarchy;
    }
    _outlineHierarchy = outlineHierarchy;
    notify(this, Document::Event::OutlineHierarchy);
}
