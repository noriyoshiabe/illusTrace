#pragma once

#include "Observable.h"
#include "Filter.h"
#include "CenterLineBuilder.h"
#include "BezierSplineBuilder.h"
#include "FeatureDetector.h"
#include "GraphBuilder.h"

#include "opencv2/imgproc.hpp"

namespace illustrace {
namespace core {

enum class IllustraceEvent {
    SourceImageLoaded,
    BrightnessFilterApplied,
    BlurFilterApplied,
    Binarized,
    Thinned,
    NegativeFilterApplied,
    CenterLineKeyPointDetected,
    CenterLineGraphBuilt,
    CenterLineGraphApproximated,
    CenterLineBuilt,
    CenterLineApproximated,
    CenterLineBezierized,
    OutlineBuilt,
    OutlineApproximated,
    OutlineBezierized,
};

class Illustrace : public lib::Observable<Illustrace, IllustraceEvent> {
public:
    Illustrace();

    bool loadSourceImage(const char *filepath);
    void binarize();

    void buildCenterLine();
    void approximateCenterLine();
    void buildBezierizedCenterLine();

    void buildOutline();
    void approximateOutline();
    void buildBezierizedOutline();

    double epsilon();

    FeatureDetector featureDetector;
    CenterLineBuilder centerLineBuilder;
    BezierSplineBuilder bezierSplineBuilder;
    GraphBuilder graphBuilder;

    cv::Rect boundingRect;

    std::vector<cv::Point2f> centerLineKeyPoints;
    Graph centerLineGraph;
    Graph approximatedCenterLineGraph;

    std::vector<std::vector<cv::Point2f>> centerLines;
    std::vector<std::vector<cv::Point2f>> approximatedCenterLines;
    std::vector<std::vector<BezierVertex<cv::Point2f>>> bezierizedCenterLines;

    std::vector<std::vector<cv::Point>> outlineContours;
    std::vector<std::vector<cv::Point2f>> approximatedOutlineContours;
    std::vector<cv::Vec4i> outlineHierarchy;
    std::vector<std::vector<BezierVertex<cv::Point2f>>> bezierizedOutlineContours;

    double brightness;
    double blur;
    double detail;
    double thickness;

    cv::Mat sourceImage;
    cv::Mat binarizedImage;
    cv::Mat thinnedImage;
    cv::Mat negativeImage;
};

} // namespace core
} // namespace illustrace
