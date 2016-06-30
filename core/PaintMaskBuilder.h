#pragma once

#include "Document.h"

namespace illustrace {

class PaintMaskBuilder {
public:
    static void build(cv::Mat &paintMask, Document *document);
};

} // namespace illustrace
