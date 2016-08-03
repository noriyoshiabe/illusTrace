#include "Illustrace.h"

#include <unordered_map>
#include <stack>

using namespace illustrace;

void Illustrace::traceForPreview(cv::Mat &sourceImage, std::vector<std::vector<cv::Point>> &outlineContours, std::vector<cv::Vec4i> &outlineHierarchy, double brightness, bool negative)
{
    double contrast = 0.0 < brightness ?  1.0 + brightness / 2.0 : 1.0;
    Filter::brightnessBGRA(sourceImage, brightness, contrast);

    cv::Mat image;
    cv::cvtColor(sourceImage, image, CV_BGRA2GRAY);

    Filter::blur(image, 3);
    Filter::threshold(image);

    if (!negative) {
        Filter::negative(image);
    }

    cv::findContours(image, outlineContours, outlineHierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
}

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
    cv::Rect contentRect = cv::Rect(0, 0, sourceImage.cols, sourceImage.rows);
    document->contentRect(contentRect);
    document->clippingRect(contentRect);

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
    if (document->negative()) {
        Filter::negative(binarizedImage);
    }

    notify(this, Illustrace::Event::Binarized, document, &binarizedImage);
    document->binarizedImage(binarizedImage);

    cv::Mat negativeImage = binarizedImage.clone();
    Filter::negative(negativeImage);

    notify(this, Illustrace::Event::NegativeFilterApplied, document, &negativeImage);
    document->negativeImage(negativeImage);

    cv::Mat preprocessedImage = negativeImage.clone();
    document->preprocessedImage(preprocessedImage);

    cv::Mat paintLayer = cv::Mat::zeros(sourceImage.rows, sourceImage.cols, CV_8UC4);
    document->paintLayer(paintLayer);
}

void Illustrace::buildLines(Document *document)
{
    cv::Mat negativeImage = document->negativeImage();

    cv::Rect boundingRect = cv::boundingRect(negativeImage);
    document->boundingRect(boundingRect);

    auto *outlineContours = new std::vector<std::vector<cv::Point>>();
    auto *outlineHierarchy = new std::vector<cv::Vec4i>();
    cv::findContours(negativeImage, *outlineContours, *outlineHierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    notify(this, Illustrace::Event::OutlineBuilt, document, outlineContours, outlineHierarchy);

    document->outlineContours(outlineContours);
    document->outlineHierarchy(outlineHierarchy);
}

void Illustrace::approximateLines(Document *document)
{
    double _epsilon = epsilon(document);

    auto *approximatedOutlineContours = new std::vector<std::vector<cv::Point2f>>();
    
    for (auto line : *document->outlineContours()) {
        std::vector<cv::Point2f> approx;
        cv::approxPolyDP(cv::Mat(line), approx, _epsilon, false);
        approximatedOutlineContours->push_back(approx);
    }

    notify(this, Illustrace::Event::OutlineApproximated, document, approximatedOutlineContours);
    document->approximatedOutlineContours(approximatedOutlineContours);
}

void Illustrace::buildPaths(Document *document)
{
    std::vector<Path *> paths;
    for (auto line : *document->approximatedOutlineContours()) {
        auto *path = new Path();
        BezierSplineBuilder::build(line, path, document->smoothing(), true, false);
        paths.push_back(path);
    }

    auto &outlineHierarchy = *document->outlineHierarchy();
    auto *hierarchyPaths = new std::vector<Path *>();
    buildPathsHierarchy(paths, nullptr, outlineHierarchy, 0, *hierarchyPaths);

    notify(this, Illustrace::Event::OutlineBezierized, document, hierarchyPaths);
    document->paths(hierarchyPaths);
}

void Illustrace::buildPathsHierarchy(std::vector<Path *> &paths, Path *parent, std::vector<cv::Vec4i> &hierarchy, int index, std::vector<Path *> &results)
{
    if (paths.empty()) {
        return;
    }

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

void Illustrace::buildPaintMask(Document *document)
{
    const cv::Mat &image = document->binarizedImage();
    cv::Mat paintMask = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);

    PaintMaskBuilder::build(paintMask, document);

    notify(this, Illustrace::Event::PaintMaskBuilt, document, &paintMask);
    document->paintMask(paintMask);
}

inline cv::Rect circleRect(cv::Point &point, int radius, cv::Mat &canvas)
{
    int minX = MAX(0, point.x - radius);
    int minY = MAX(0, point.y - radius);
    int maxX = MIN(point.y + radius, canvas.cols - 1);
    int maxY = MIN(point.y + radius, canvas.rows - 1);
    return cv::Rect(minX, minY, maxX - minX + 1, maxY - minY + 1);
}

void Illustrace::drawCircleOnPreprocessedImage(cv::Point &point, int radius, int color, Document *document)
{
    cv::Mat &preprocessedImage = document->preprocessedImage();
    cv::circle(preprocessedImage, point, radius, cv::Scalar(color), -1);

    auto dirtyRect = circleRect(point, radius, preprocessedImage);
    notify(this, Illustrace::Event::PreprocessedImageUpdated, document, &preprocessedImage, &dirtyRect);
    document->preprocessedImage(preprocessedImage, &dirtyRect);
}

void Illustrace::eraseCircleOnPreprocessedImage(cv::Point &point, int radius, Document *document)
{
    cv::Mat &preprocessedImage = document->preprocessedImage();
    cv::Mat &binarizedImage = document->binarizedImage();
    uint8_t *dstData = (uint8_t *)preprocessedImage.data;
    uint8_t *srcData = (uint8_t *)binarizedImage.data;

    int sideLength = radius * 2 + 1;
    cv::Mat circle = cv::Mat::zeros(sideLength, sideLength, CV_8UC1);
    cv::circle(circle, cv::Point(radius, radius), radius, cv::Scalar(255), -1);

    int dstStartFromX = point.x - radius;
    int dstStartFromY = point.y - radius;

    for (int y = 0; y < sideLength; ++y) {
        int dstY = dstStartFromY + y;
        if (0 <= dstY && dstY < preprocessedImage.rows) {
            int yOffset = sideLength * y;
            int yDstOffset = dstY * preprocessedImage.cols;
            for (int x = 0; x < sideLength; ++x) {
                int dstX = dstStartFromX + x;
                if (0 <= dstX && dstX < preprocessedImage.cols) {
                    if (0 != circle.data[yOffset + x]) {
                        dstData[yDstOffset + dstX] = srcData[yDstOffset + dstX];
                    }
                }
            }
        }
    }

    auto dirtyRect = circleRect(point, radius, preprocessedImage);
    notify(this, Illustrace::Event::PreprocessedImageUpdated, document, &preprocessedImage, &dirtyRect);
    document->preprocessedImage(preprocessedImage, &dirtyRect);
}

void Illustrace::drawCircleOnPaintLayer(cv::Point &point, int radius, cv::Scalar &color, Document *document)
{
    bool changed = false;

    cv::Mat &paintLayer = document->paintLayer();
    uint32_t *data = (uint32_t *)paintLayer.data;

    cv::Mat &paintMask = document->paintMask();
    uint8_t *paintMaskData = paintMask.data;

    uint32_t newColor = (int)color[0] | (int)color[1] << 8 | (int)color[2] << 16 | (int)color[3] << 24;

    int sideLength = radius * 2 + 1;
    cv::Mat circle = cv::Mat::zeros(sideLength, sideLength, CV_8UC1);

    cv::circle(circle, cv::Point(radius, radius), radius, cv::Scalar(255), -1);

    int dstStartFromX = point.x - radius;
    int dstStartFromY = point.y - radius;

    for (int y = 0; y < sideLength; ++y) {
        int dstY = dstStartFromY + y;
        if (0 <= dstY && dstY < paintLayer.rows) {
            int yOffset = sideLength * y;
            int yDstOffset = dstY * paintLayer.cols;
            for (int x = 0; x < sideLength; ++x) {
                int dstX = dstStartFromX + x;
                if (0 <= dstX && dstX < paintLayer.cols) {
                    if (0 != circle.data[yOffset + x] && 0 == paintMaskData[yDstOffset + dstX]) {
                        if (data[yDstOffset + dstX] != newColor) {
                            data[yDstOffset + dstX] = newColor;
                            changed = true;
                        }
                    }
                }
            }
        }
    }

    if (changed) {
        auto dirtyRect = circleRect(point, radius, paintLayer);
        notify(this, Illustrace::Event::PaintLayerUpdated, document, &paintLayer, &dirtyRect);
        document->paintLayer(paintLayer, &dirtyRect);
    }
}

void Illustrace::fillRegionOnPaintLayer(cv::Point &seed, cv::Scalar &color, Document *document)
{
    // Based on Scanline Floodfill Algorithm With Stack (floodFillScanlineStack)
    // http://lodev.org/cgtutor/floodfill.html

    cv::Mat &paintLayer = document->paintLayer();
    uint32_t *data = (uint32_t *)paintLayer.data;

    cv::Mat &paintMask = document->paintMask();
    uint8_t *paintMaskData = paintMask.data;

    uint32_t newColor = (int)color[0] | (int)color[1] << 8 | (int)color[2] << 16 | (int)color[3] << 24;
    uint32_t oldColor = data[seed.y * paintLayer.cols + seed.x];

    if (oldColor == newColor || 0 != paintMaskData[seed.y * paintLayer.cols + seed.x]) {
        return;
    }

    int minX = seed.x;
    int minY = seed.y;
    int maxX = seed.x;
    int maxY = seed.y;

    std::stack<cv::Point> stack;

    stack.push(seed);

    while(!stack.empty()) {
        cv::Point pt = stack.top();
        stack.pop();

        int yOffset = pt.y * paintLayer.cols;
        int yOffsetMinus1 = yOffset - paintLayer.cols;
        int yOffsetPlus1 = yOffset + paintLayer.cols;

        minX = MIN(pt.y, minY);
        maxY = MAX(pt.y, maxY);

        while (0 <= pt.x && data[yOffset + pt.x] == oldColor && 0 == paintMaskData[yOffset + pt.x]) {
            --pt.x;
        }

        minX = MIN(pt.x, minX);

        ++pt.x;

        bool spanAbove = false;
        bool spanBelow = false;

        while (pt.x < paintLayer.cols && data[yOffset + pt.x] == oldColor && 0 == paintMaskData[yOffset + pt.x]) {
            data[yOffset + pt.x] = newColor;

            if (0 < pt.y) {
                if (!spanAbove && data[yOffsetMinus1 + pt.x] == oldColor && 0 == paintMaskData[yOffsetMinus1 + pt.x]) {
                    stack.push(cv::Point(pt.x, pt.y - 1));
                    spanAbove = true;
                }
                else if(spanAbove && (data[yOffsetMinus1 + pt.x] != oldColor || 0 != paintMaskData[yOffsetMinus1 + pt.x])) {
                    spanAbove = false;
                }
            }

            if (pt.y < paintLayer.rows - 1) {
                if (!spanBelow && data[yOffsetPlus1 + pt.x] == oldColor && 0 == paintMaskData[yOffsetPlus1 + pt.x]) {
                    stack.push(cv::Point(pt.x, pt.y + 1));
                    spanBelow = true;
                }
                else if (spanBelow && (data[yOffsetPlus1 + pt.x] != oldColor || 0 != paintMaskData[yOffsetPlus1 + pt.x])) {
                    spanBelow = false;
                }
            }

            ++pt.x;
        }

        maxX = MAX(pt.x, maxX);
    }

    auto dirtyRect = cv::Rect(minX, minY, maxX - minX + 1, maxY - minY + 1);
    notify(this, Illustrace::Event::PaintLayerUpdated, document, &paintLayer, &dirtyRect);
    document->paintLayer(paintLayer, &dirtyRect);
}

void Illustrace::buildPaintPaths(Document *document)
{
    std::unordered_map<uint32_t, std::vector<cv::Point>> paintMap;

    cv::Mat &paintLayer = document->paintLayer();
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

        Filter::blur(negativeImage, 5);
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(negativeImage, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

        std::vector<std::vector<cv::Point2f>> approximatedLines;

        for (auto line : contours) {
            std::vector<cv::Point2f> approx;
            cv::approxPolyDP(cv::Mat(line), approx, 1, false);
            approximatedLines.push_back(approx);
        }

        std::vector<Path *> paths;

        for (auto line : approximatedLines) {
            auto *path = new Path();
            path->color = new cv::Scalar(color[0], color[1], color[2], color[3]);
            BezierSplineBuilder::build(line, path, document->smoothing(), true, true);
            paths.push_back(path);
        }

        buildPathsHierarchy(paths, nullptr, hierarchy, 0, *hierarchyPaths);
    }

    notify(this, Illustrace::Event::PaintPathsBuilt, document, hierarchyPaths);
    document->paintPaths(hierarchyPaths);
}

int Illustrace::blur(cv::Mat &sourceImage, Document *document)
{
    int blur = document->blur() * 5;
    return 0 == blur % 2 ? blur + 1 : blur;
}

double Illustrace::epsilon(Document *document)
{
    return 1.2 / document->detail();
}
