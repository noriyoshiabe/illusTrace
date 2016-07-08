#pragma once

#include "Observable.h"
#include "Filter.h"
#include "BezierSplineBuilder.h"
#include "PaintMaskBuilder.h"
#include "Document.h"

#include "opencv2/imgproc.hpp"

namespace illustrace {

class Illustrace : public Observable<Illustrace> {
public:
    enum Event : int {
        SourceImageLoaded,
        BrightnessFilterApplied,
        BlurFilterApplied,
        Binarized,
        NegativeFilterApplied,
        OutlineBuilt,
        OutlineApproximated,
        OutlineBezierized,
        PaintMaskBuilt,
        PaintLayerUpdated,
        PaintPathsBuilt,
        PreprocessedImageUpdated,
    };

    void traceForPreview(cv::Mat &sourceImage, std::vector<std::vector<cv::Point>> &outlineContours, std::vector<cv::Vec4i> &outlineHierarchy, double brightness, bool negative = false);
    bool traceFromFile(const char *filepath, Document *document);
    void traceFromImage(cv::Mat &sourceImage, Document *document);
    void binarize(cv::Mat &sourceImage, Document *document);
    void buildLines(Document *document);
    void approximateLines(Document *document);
    void buildPaths(Document *document);
    void buildPaintMask(Document *document);
    void drawCircleOnPreprocessedImage(cv::Point &point, int radius, int color, Document *document);
    void eraseCircleOnPreprocessedImage(cv::Point &point, int radius, Document *document);
    void drawCircleOnPaintLayer(cv::Point &point, int radius, cv::Scalar &color, Document *document);
    void fillRegionOnPaintLayer(cv::Point &seed, cv::Scalar &color, Document *document);
    void buildPaintPaths(Document *document);

    static inline const char *Event2CString(Event event) {
#define CASE(event) case event: return #event
        switch (event) {
        CASE(SourceImageLoaded);
        CASE(BrightnessFilterApplied);
        CASE(BlurFilterApplied);
        CASE(Binarized);
        CASE(NegativeFilterApplied);
        CASE(OutlineBuilt);
        CASE(OutlineApproximated);
        CASE(OutlineBezierized);
        CASE(PaintMaskBuilt);
        CASE(PaintLayerUpdated);
        CASE(PaintPathsBuilt);
        CASE(PreprocessedImageUpdated);
        }
#undef CASE
    }


private:
    void buildPathsHierarchy(std::vector<Path *> &paths, Path *parent, std::vector<cv::Vec4i> &hierarchy, int index, std::vector<Path *> &results);
    int blur(cv::Mat &sourceImage, Document *document);
    double epsilon(Document *document);
};

} // namespace illustrace
