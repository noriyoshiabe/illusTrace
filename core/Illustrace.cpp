#include "Illustrace.h"

using namespace illustrace;

bool Illustrace::traceFromFile(const char *filepath, Document *document)
{
    cv::Mat sourceImage = imread(filepath, cv::IMREAD_GRAYSCALE);
    if (!sourceImage.data) {
        return false;
    }

    notify(this, Illustrace::Event::SourceImageLoaded, &sourceImage);
    
    document->sourceImage(sourceImage);

    binarize(document);
    buildLines(document);
    buildPaths(document);

    return true;
}

void Illustrace::binarize(Document *document)
{
    cv::Mat binarizedImage = document->sourceImage().clone();

    Filter::brightness(binarizedImage, document->brightness());
    notify(this, Illustrace::Event::BrightnessFilterApplied, &binarizedImage);

    Filter::blur(binarizedImage, blur(document));
    notify(this, Illustrace::Event::BlurFilterApplied, &binarizedImage);

    Filter::threshold(binarizedImage);
    notify(this, Illustrace::Event::Binarized, &binarizedImage);

    document->binarizedImage(binarizedImage);

    cv::Mat negativeImage = binarizedImage.clone();
    Filter::negative(negativeImage);
    notify(this, Illustrace::Event::NegativeFilterApplied, &negativeImage);

    document->negativeImage(negativeImage);

    cv::Rect boundingRect = cv::boundingRect(negativeImage);
    document->boundingRect(boundingRect);
}

void Illustrace::buildLines(Document *document)
{
    if (LineMode::Center == document->mode()) {
        cv::Mat thinnedImage = document->binarizedImage().clone();
        Filter::thinning(thinnedImage);
        notify(this, Illustrace::Event::Thinned, &thinnedImage);

        std::vector<cv::Point2f> keyPoints;
        FeatureDetector::detect(thinnedImage, keyPoints);
        notify(this, Illustrace::Event::CenterLineKeyPointDetected, &keyPoints);

        Graph graph;
        GraphBuilder::build(thinnedImage, keyPoints, graph);
        notify(this, Illustrace::Event::CenterLineGraphBuilt, &graph);

        Graph approximatedGraph;
        GraphBuilder::approximate(graph, approximatedGraph);
        notify(this, Illustrace::Event::CenterLineGraphApproximated, &approximatedGraph);

        auto *centerLines = new std::vector<std::vector<cv::Point2f>>();
        CenterLineBuilder::build(approximatedGraph, *centerLines);
        notify(this, Illustrace::Event::CenterLineBuilt, centerLines);

        document->centerLines(centerLines);
    }
    else {
        auto *outlineContours = new std::vector<std::vector<cv::Point>>();
        auto *outlineHierarchy = new std::vector<cv::Vec4i>();
        cv::findContours(document->negativeImage(), *outlineContours, *outlineHierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
        notify(this, Illustrace::Event::OutlineBuilt, outlineContours, outlineHierarchy);

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

        notify(this, Illustrace::Event::CenterLineApproximated, approximatedCenterLines);
        document->approximatedCenterLines(approximatedCenterLines);
    }
    else {
        auto *approximatedOutlineContours = new std::vector<std::vector<cv::Point2f>>();
        
        for (auto line : *document->outlineContours()) {
            std::vector<cv::Point2f> approx;
            cv::approxPolyDP(cv::Mat(line), approx, _epsilon, false);
            approximatedOutlineContours->push_back(approx);
        }

        notify(this, Illustrace::Event::OutlineApproximated, approximatedOutlineContours);
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

        notify(this, Illustrace::Event::CenterLineBezierized, paths);
    }
    else {
        auto *paths = new std::vector<Path *>();
        for (auto line : *document->approximatedOutlineContours()) {
            auto *path = new Path();
            BezierSplineBuilder::build(line, path, document->smoothing(), true);
            paths->push_back(path);
        }

        notify(this, Illustrace::Event::OutlineBezierized, paths);
    }
}

int Illustrace::blur(Document *document)
{
    cv::Rect boundingRect = document->boundingRect();
    double shortSide = MIN(boundingRect.width, boundingRect.height);
    int blur = shortSide * document->blur();
    return 0 == blur % 2 ? blur + 1 : blur;
}

double Illustrace::epsilon(Document *document)
{
    cv::Rect boundingRect = document->boundingRect();
    double shortSide = MIN(boundingRect.width, boundingRect.height);
    return shortSide * (0.006250 / document->detail()); // Inverse proportion. 0.5 to 1.25%
}
