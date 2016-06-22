#pragma once

#include "Document.h"
#include "Illustrace.h"

namespace illustrace {

class Editor {
public:
    Editor(Illustrace *illustrace, Document *document);
    ~Editor();

    void detail(double detail);
    void thickness(double thickness);
    void scale(double scale);
    void rotation(double rotation);
    void clippingRect(cv::Rect &rect);
    void color(cv::Scalar &color);
    void backgroundColor(cv::Scalar &backgroundColor);

private:
    Illustrace *illustrace;
    Document *document;
};

} // namespace illustrace
