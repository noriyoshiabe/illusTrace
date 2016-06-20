#pragma once

#include "Observable.h"
#include "Filter.h"
#include "CenterLineBuilder.h"
#include "BezierSplineBuilder.h"
#include "FeatureDetector.h"
#include "GraphBuilder.h"
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

    bool traceFromFile(const char *filepath, Document *document);
    void binarize(Document *document);
    void buildLines(Document *document);
    void approximateLines(Document *document);
    void buildPaths(Document *document);

private:
    int blur(Document *document);
    double epsilon(Document *document);
};

} // namespace illustrace
