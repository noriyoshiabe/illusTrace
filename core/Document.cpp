#include "Document.h"

using namespace illustrace;

Document::Document() :
    _mode(LineMode::Center),
    _brightness(0.0),
    _blur(0.015),
    _detail(1.0),
    _smoothing(1.0),
    _thickness(1.0),
    _scale(1.0),
    _rotation(0.0),
    _color(cv::Scalar(0, 0, 0, 255)),
    _backgroundColor(cv::Scalar(0, 0, 0, 0)),
    _paths(nullptr),
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
    }

    delete _paths;

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

double Document::scale()
{
    return _scale;
}

double Document::rotation()
{
    return _rotation;
}

cv::Scalar &Document::color()
{
    return _color;
}

cv::Scalar &Document::backgroundColor()
{
    return _backgroundColor;
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

cv::Mat &Document::sourceImage()
{
    return _sourceImage;
}

cv::Mat &Document::binarizedImage()
{
    return _binarizedImage;
}

cv::Mat &Document::negativeImage()
{
    return _negativeImage;
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

void Document::scale(double scale)
{
    _scale = scale;
    notify(this, Document::Event::Scale);
}

void Document::rotation(double rotation)
{
    _rotation = rotation;
    notify(this, Document::Event::Rotation);
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
    }
    _paths = paths;
    notify(this, Document::Event::Paths);
}

void Document::sourceImage(cv::Mat &sourceImage)
{
    _sourceImage = sourceImage;
    notify(this, Document::Event::SourceImage);
}

void Document::binarizedImage(cv::Mat &binarizedImage)
{
    _binarizedImage = binarizedImage;
    notify(this, Document::Event::BinarizedImage);
}

void Document::negativeImage(cv::Mat &negativeImage)
{
    _negativeImage = negativeImage;
    notify(this, Document::Event::NegativeImage);
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

cv::Size Document::scaledSize()
{
    return (cv::Size){(int)round(_clippingRect.width * _scale), (int)round(_clippingRect.height * _scale)};
}
