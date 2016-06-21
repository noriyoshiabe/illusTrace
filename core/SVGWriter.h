#pragma once

#include "Document.h"

namespace illustrace {

class SVGWriter {
public:
    static bool write(const char *filepath, Document *document, const char *comment);
};

} // namespace illustrace
