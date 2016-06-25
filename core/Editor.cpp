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

void Editor::rotation(double rotation)
{
    document->rotation(rotation);
}

void Editor::clippingRect(cv::Rect &rect)
{
    document->clippingRect(rect);
}

void Editor::color(cv::Scalar &color)
{
    document->color(color);
}

void Editor::backgroundColor(cv::Scalar &backgroundColor)
{
    document->backgroundColor(backgroundColor);
}
