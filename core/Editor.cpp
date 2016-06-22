#include "Editor.h"

using namespace illustrace;

Editor::Editor(Illustrace *illustrace, Document *document)
    : illustrace(illustrace), document(document)
{
}

Editor::~Editor()
{
}

void Editor::detail(double detail)
{
    document->detail(detail);
    illustrace->approximateLines(document);
    illustrace->buildPaths(document);
}

void Editor::thickness(double thickness)
{
    document->thickness(thickness);
}

void Editor::scale(double scale)
{
    document->scale(scale);
}

void Editor::rotation(double rotation)
{
    document->rotation(rotation);
}

void Editor::clippingRect(cv::Rect &rect)
{
    document->clippingRect(rect);
}

void Editor::stroke(cv::Scalar &stroke)
{
    document->stroke(stroke);
}

void Editor::fill(cv::Scalar &fill)
{
    document->fill(fill);
}
