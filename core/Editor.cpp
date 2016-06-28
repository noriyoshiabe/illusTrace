#include "Editor.h"

using namespace illustrace;

Editor::Editor(Illustrace *illustrace, Document *document)
    : illustrace(illustrace), document(document),
      _mode(Editor::Mode::Line), _lineState(Editor::LineState::Line),
      _paintState(Editor::PaintState::Brush),
      _clipState(Editor::ClipState::Trimming)
{
}

Editor::~Editor()
{
}

Editor::Mode Editor::mode()
{
    return _mode;
}

Editor::LineState Editor::lineState()
{
    return _lineState;
}

Editor::PaintState Editor::paintState()
{
    return _paintState;
}

Editor::ClipState Editor::clipState()
{
    return _clipState;
}

void Editor::mode(Mode mode)
{
    _mode = mode;
    notify(this, Event::Mode);
}

void Editor::lineState(LineState state)
{
    _lineState = state;
    notify(this, Event::LineState);
}

void Editor::paintState(PaintState state)
{
    _paintState = state;
    notify(this, Event::PaintState);
}

void Editor::clipState(ClipState state)
{
    _clipState = state;
    notify(this, Event::ClipState);
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

void Editor::backgroundEnable(bool enable)
{
    document->backgroundEnable(enable);
}
