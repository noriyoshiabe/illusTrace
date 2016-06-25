#include "Illustrace.h"

using namespace illustrace;

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
        buildOutlinePathsHierarchy(paths, nullptr, outlineHierarchy, 0, *hierarchyPaths);

        notify(this, Illustrace::Event::OutlineBezierized, document, hierarchyPaths);
        document->paths(hierarchyPaths);
    }
}

void Illustrace::buildOutlinePathsHierarchy(std::vector<Path *> &paths, Path *parent, std::vector<cv::Vec4i> &outlineHierarchy, int index, std::vector<Path *> &results)
{
    for (; -1 != index; index = outlineHierarchy[index][0]) {
        Path *path = paths[index];
        if (parent) {
            parent->children.push_back(path);
        }
        else {
            results.push_back(path);
        }

        int childIndex = outlineHierarchy[index][2];
        if (-1 != childIndex) {
            buildOutlinePathsHierarchy(paths, path, outlineHierarchy, childIndex, results);
        }
    }
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
