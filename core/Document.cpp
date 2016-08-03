#include "Document.h"

using namespace illustrace;

Document::Document() :
    _brightness(0.0),
    _negative(false),
    _blur(1.0),
    _detail(1.0),
    _smoothing(1.0),
    _thickness(1.0),
    _rotation(0.0),
    _color(cv::Scalar(0, 0, 0)),
    _backgroundColor(cv::Scalar(255, 255, 255)),
    _backgroundEnable(false),
    _paths(nullptr),
    _paintPaths(nullptr),
    _outlineContours(nullptr),
    _approximatedOutlineContours(nullptr),
    _outlineHierarchy(nullptr)
{
    _paths = new std::vector<Path *>();
    _paintPaths = new std::vector<Path *>();
    _outlineContours = new std::vector<std::vector<cv::Point>>();
    _approximatedOutlineContours = new std::vector<std::vector<cv::Point2f>>();
    _outlineHierarchy = new std::vector<cv::Vec4i>();
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

double Document::brightness()
{
    return _brightness;
}

bool Document::negative()
{
    return _negative;
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

cv::Rect &Document::contentRect()
{
    return _contentRect;
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

cv::Mat &Document::negativeImage()
{
    return _negativeImage;
}

cv::Mat &Document::preprocessedImage()
{
    return _preprocessedImage;
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

void Document::brightness(double brightness)
{
    _brightness = brightness;
    notify(this, Document::Event::Brightness);
}

void Document::negative(bool negative)
{
    _negative = negative;
    notify(this, Document::Event::Negative);
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

void Document::backgroundEnable(bool backgroundEnable)
{
    _backgroundEnable = backgroundEnable;
    notify(this, Document::Event::BackgroundEnable);
}

void Document::paintLayer(cv::Mat &paintLayer)
{
    _paintLayer = paintLayer;
    notify(this, Document::Event::PaintLayer, &_contentRect);
}

void Document::paintLayer(cv::Mat &paintLayer, cv::Rect *dirtyRect)
{
    _paintLayer = paintLayer;
    notify(this, Document::Event::PaintLayer, dirtyRect);
}

void Document::paintMask(cv::Mat &paintMask)
{
    _paintMask = paintMask;
    notify(this, Document::Event::PaintMask);
}

void Document::contentRect(cv::Rect &rect)
{
    _contentRect = rect;
    notify(this, Document::Event::ContentRect);
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

void Document::negativeImage(cv::Mat &negativeImage)
{
    _negativeImage = negativeImage;
    notify(this, Document::Event::NegativeImage);
}

void Document::preprocessedImage(cv::Mat &preprocessedImage)
{
    _preprocessedImage = preprocessedImage;
    notify(this, Document::Event::PreprocessedImage, &_contentRect);
}

void Document::preprocessedImage(cv::Mat &preprocessedImage, cv::Rect *dirtyRect)
{
    _preprocessedImage = preprocessedImage;
    notify(this, Document::Event::PreprocessedImage, dirtyRect);
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

namespace illustrace {

std::ostream &operator<<(std::ostream &os, Document const &self)
{
    os << "<Document ";
    os << "brightness: " << self._brightness << ", ";
    os << "negative: " << self._negative << ", ";
    os << "blur: " << self._blur << ", ";
    os << "detail: " << self._detail << ", ";
    os << "smoothing: " << self._smoothing << ", ";
    os << "thickness: " << self._thickness << ", ";
    os << "rotation: " << self._rotation << ", ";
    os << "color: " << self._color << ", ";
    os << "backgroundColor: " << self._backgroundColor << ", ";
    os << "paintLayer: " << &self._paintLayer << ", ";
    os << "paintMask: " << &self._paintMask << ", ";
    os << "backgroundEnable: " << self._backgroundEnable << ", ";
    os << "contentRect: " << self._contentRect << ", ";
    os << "clippingRect: " << self._clippingRect << ", ";
    os << "boundingRect: " << self._boundingRect << ", ";
    os << "paths: " << self._paths << ", ";
    os << "paintPaths: " << self._paintPaths << ", ";
    os << "binarizedImage: " << &self._binarizedImage << ", ";
    os << "negativeImage: " << &self._negativeImage << ", ";
    os << "preprocessedImage: " << &self._preprocessedImage << ", ";
    os << "outlineContours: " << self._outlineContours << ", ";
    os << "approximatedOutlineContours: " << self._approximatedOutlineContours << ", ";
    os << "outlineHierarchy: " << self._outlineHierarchy << "";
    os << ">";
    return os;
}

} // namespace illustrace
