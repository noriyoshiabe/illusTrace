#include "Illustrace.h"

#include <unordered_map>
#include "cairo/cairo.h"

using namespace illustrace;

static void drawPaths(cairo_t *cr, std::vector<Path *> *paths);
static void drawPath(cairo_t *cr, Path *path);

bool Illustrace::traceFromFile(const char *filepath, Document *document)
{
    cv::Mat sourceImage = imread(filepath, cv::IMREAD_GRAYSCALE);
    if (!sourceImage.data) {
        return false;
    }

    notify(this, Illustrace::Event::SourceImageLoaded, document, &sourceImage);

    traceFromImage(sourceImage, document);   
    return true;
}

void Illustrace::traceFromImage(cv::Mat &sourceImage, Document *document)
{
    cv::Rect clippingRect = cv::Rect(0, 0, sourceImage.cols, sourceImage.rows);
    document->clippingRect(clippingRect);

    binarize(sourceImage, document);
    buildLines(document);
    approximateLines(document);
    buildPaths(document);
}

void Illustrace::binarize(cv::Mat &sourceImage, Document *document)
{
    cv::Mat binarizedImage = sourceImage.clone();

    Filter::brightness(binarizedImage, document->brightness());
    notify(this, Illustrace::Event::BrightnessFilterApplied, document, &binarizedImage);

    Filter::blur(binarizedImage, blur(sourceImage, document));
    notify(this, Illustrace::Event::BlurFilterApplied, document, &binarizedImage);

    Filter::threshold(binarizedImage);
    notify(this, Illustrace::Event::Binarized, document, &binarizedImage);

    document->binarizedImage(binarizedImage);
    document->preprocessedImage(binarizedImage);

    cv::Mat paintLayer = cv::Mat(sourceImage.rows, sourceImage.cols, CV_8UC3);
    document->paintLayer(paintLayer);
}

void Illustrace::buildLines(Document *document)
{
    cv::Mat preprocessedImage = document->preprocessedImage();
    cv::Mat negativeImage = preprocessedImage.clone();
    Filter::negative(negativeImage);
    notify(this, Illustrace::Event::NegativeFilterApplied, document, &negativeImage);

    cv::Rect boundingRect = cv::boundingRect(negativeImage);
    document->boundingRect(boundingRect);

    if (LineMode::Center == document->mode()) {
        cv::Mat thinnedImage = preprocessedImage.clone();
        Filter::thinning(thinnedImage);
        notify(this, Illustrace::Event::Thinned, document, &thinnedImage);

        std::vector<cv::Point2f> keyPoints;
        FeatureDetector::detect(thinnedImage, keyPoints);
        notify(this, Illustrace::Event::CenterLineKeyPointDetected, document, &thinnedImage, &keyPoints);

        Graph graph;
        GraphBuilder::build(thinnedImage, keyPoints, graph);
        notify(this, Illustrace::Event::CenterLineGraphBuilt, document, &graph);

        Graph approximatedGraph;
        GraphBuilder::approximate(graph, approximatedGraph);
        notify(this, Illustrace::Event::CenterLineGraphApproximated, document, &approximatedGraph);

        auto *centerLines = new std::vector<std::vector<cv::Point2f>>();
        CenterLineBuilder::build(approximatedGraph, *centerLines);
        notify(this, Illustrace::Event::CenterLineBuilt, document, centerLines);

        document->centerLines(centerLines);
    }
    else {
        auto *outlineContours = new std::vector<std::vector<cv::Point>>();
        auto *outlineHierarchy = new std::vector<cv::Vec4i>();
        cv::findContours(negativeImage, *outlineContours, *outlineHierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
        notify(this, Illustrace::Event::OutlineBuilt, document, outlineContours, outlineHierarchy);

        document->outlineContours(outlineContours);
        document->outlineHierarchy(outlineHierarchy);
    }
}

void Illustrace::approximateLines(Document *document)
{
    double _epsilon = epsilon(document);

    if (LineMode::Center == document->mode()) {
        auto *approximatedCenterLines = new std::vector<std::vector<cv::Point2f>>();    

        for (auto line : *document->centerLines()) {
            std::vector<cv::Point2f> approx;
            bool needAdjust = line.front() == line.back();
            cv::approxPolyDP(cv::Mat(line), approx, _epsilon, false);
            if (needAdjust && approx.front() != approx.back()) {
                approx.push_back(approx.front());
            }
            approximatedCenterLines->push_back(approx);
        }

        notify(this, Illustrace::Event::CenterLineApproximated, document, approximatedCenterLines);
        document->approximatedCenterLines(approximatedCenterLines);
    }
    else {
        auto *approximatedOutlineContours = new std::vector<std::vector<cv::Point2f>>();
        
        for (auto line : *document->outlineContours()) {
            std::vector<cv::Point2f> approx;
            cv::approxPolyDP(cv::Mat(line), approx, _epsilon, false);
            approximatedOutlineContours->push_back(approx);
        }

        notify(this, Illustrace::Event::OutlineApproximated, document, approximatedOutlineContours);
        document->approximatedOutlineContours(approximatedOutlineContours);
    }
}

void Illustrace::buildPaths(Document *document)
{
    if (LineMode::Center == document->mode()) {
        auto *paths = new std::vector<Path *>();

        for (auto line : *document->approximatedCenterLines()) {
            auto *path = new Path();
            BezierSplineBuilder::build(line, path, document->smoothing());
            paths->push_back(path);
        }

        notify(this, Illustrace::Event::CenterLineBezierized, document, paths);
        document->paths(paths);
    }
    else {
        std::vector<Path *> paths;
        for (auto line : *document->approximatedOutlineContours()) {
            auto *path = new Path();
            BezierSplineBuilder::build(line, path, document->smoothing(), true);
            paths.push_back(path);
        }

        auto &outlineHierarchy = *document->outlineHierarchy();
        auto *hierarchyPaths = new std::vector<Path *>();
        buildPathsHierarchy(paths, nullptr, outlineHierarchy, 0, *hierarchyPaths);

        notify(this, Illustrace::Event::OutlineBezierized, document, hierarchyPaths);
        document->paths(hierarchyPaths);
    }
}

void Illustrace::buildPathsHierarchy(std::vector<Path *> &paths, Path *parent, std::vector<cv::Vec4i> &hierarchy, int index, std::vector<Path *> &results)
{
    for (; -1 != index; index = hierarchy[index][0]) {
        Path *path = paths[index];
        if (parent) {
            parent->children.push_back(path);
        }
        else {
            results.push_back(path);
        }

        int childIndex = hierarchy[index][2];
        if (-1 != childIndex) {
            buildPathsHierarchy(paths, path, hierarchy, childIndex, results);
        }
    }
}

void Illustrace::buildPaintMask(std::vector<Path *> *paths, Document *document)
{
    const cv::Mat &image = document->binarizedImage();
    cv::Mat paintMask = cv::Mat(image.rows, image.cols, CV_8UC1);

    cairo_surface_t *surface = cairo_image_surface_create_for_data(paintMask.data, CAIRO_FORMAT_A8,
            paintMask.cols, paintMask.rows, cairo_format_stride_for_width(CAIRO_FORMAT_A8, paintMask.cols));
    cairo_t *cr = cairo_create(surface);
   
    cairo_set_line_width(cr, document->thickness());
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);

    drawPaths(cr, paths);

    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    notify(this, Illustrace::Event::PaintMaskBuilt, document, &paintMask);
    document->paintMask(paintMask);
}

void Illustrace::fillPaintLayer(cv::Point &seed, cv::Scalar &color, Document *document)
{
    cv::Mat &paintLayer = document->paintLayer();
    cv::floodFill(paintLayer, document->paintMask(), seed, color);
    notify(this, Illustrace::Event::PaintLayerUpdated, document, &paintLayer);
    document->paintLayer(paintLayer);
}

void Illustrace::buildPaintPaths(Document *document)
{
    std::unordered_map<uint32_t, std::vector<cv::Point>> paintMap;

    cv::Mat paintLayer = document->paintLayer();
    uint32_t *data = (uint32_t *)paintLayer.data;
    for (int y = 0; y < paintLayer.rows; ++y) {
        int yOffset = y * paintLayer.cols;
        for (int x = 0; x < paintLayer.cols; ++x) {
            uint32_t color = data[yOffset + x];
            if (paintMap.find(color) == paintMap.end()) {
                paintMap[color] = std::vector<cv::Point>();
            }

            if (((uint8_t *)&color)[3]) {
                paintMap[color].push_back((cv::Point){x, y});
            }
        }
    }

    cv::Mat negativeImage = cv::Mat(paintLayer.rows, paintLayer.cols, CV_8UC1);
    auto *hierarchyPaths = new std::vector<Path *>();

    for (auto entry : paintMap) {
        const uint8_t *color = (const uint8_t *)&entry.first;
        std::vector<cv::Point> &paintedPixels = entry.second;

        int length = negativeImage.rows * negativeImage.cols;
        for (int i = 0; i < length; ++i) {
            negativeImage.data[i] = 0;
        }

        for (auto point : paintedPixels) {
            negativeImage.data[point.y * negativeImage.cols + point.x] = 255;
        }

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(negativeImage, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

        std::vector<Path *> paths;

        for (auto line : contours) {
            auto *path = new Path();
            path->color = new cv::Scalar(color[0], color[1], color[2]);
            PolylineBuilder::build(line, path, true);
            paths.push_back(path);
        }

        buildPathsHierarchy(paths, nullptr, hierarchy, 0, *hierarchyPaths);
    }

    notify(this, Illustrace::Event::PaintPathsBuilt, document, hierarchyPaths);
    document->paintPaths(hierarchyPaths);
}

int Illustrace::blur(cv::Mat &sourceImage, Document *document)
{
    double shortSide = MIN(sourceImage.cols, sourceImage.rows);
    int blur = shortSide * document->blur();
    return 0 == blur % 2 ? blur + 1 : blur;
}

double Illustrace::epsilon(Document *document)
{
    cv::Rect boundingRect = document->boundingRect();
    double shortSide = MIN(boundingRect.width, boundingRect.height);
    return shortSide * (0.006250 / document->detail()); // Inverse proportion. 0.5 to 1.25%
}

// Local functions for Cairo

static void drawPaths(cairo_t *cr, std::vector<Path *> *paths)
{
    for (auto *path : *paths) {
        drawPath(cr, path);

        if (path->closed) {
            cairo_fill_preserve(cr);
        }

        cairo_stroke(cr);
    }
}

static void drawPath(cairo_t *cr, Path *path)
{
    cairo_new_sub_path(cr);

    for (Segment &s : path->segments) {
        switch (s.type) {
        case Segment::Type::Move:
            cairo_move_to(cr, s[2].x, s[2].y);
            break;
        case Segment::Type::Line:
            cairo_line_to(cr, s[2].x, s[2].y);
            break;
        case Segment::Type::Curve:
            cairo_curve_to(cr, s[0].x, s[0].y, s[1].x, s[1].y, s[2].x, s[2].y); 
            break;
        }
    }

    if (path->closed) {
        cairo_close_path(cr);
    }

    for (Path *child : path->children) {
        drawPath(cr, child);
    }
}
