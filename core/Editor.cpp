#include "Editor.h"

using namespace illustrace;

class Editor::Command {
public:
    Command(Editor *editor) : document(editor->document), illustrace(editor->illustrace) {};
    virtual ~Command() {}
    virtual void execute() = 0;
    virtual void undo() = 0;

    Document *document;
    Illustrace *illustrace;
};

class DetailCommand : public Editor::Command {
public:
    DetailCommand(Editor *editor) : Command(editor) {}

    double oldValue;
    double newValue;

    void execute() {
        document->detail(newValue);
        illustrace->approximateLines(document);
        illustrace->buildPaths(document);
    }

    void undo() {
        document->detail(oldValue);
        illustrace->approximateLines(document);
        illustrace->buildPaths(document);
    }
};

class ThicknessCommand : public Editor::Command {
public:
    ThicknessCommand(Editor *editor) : Command(editor) {}

    double oldValue;
    double newValue;

    void execute() {
        document->thickness(newValue);
    }

    void undo() {
        document->thickness(oldValue);
    }
};

class RotationCommand : public Editor::Command {
public:
    RotationCommand(Editor *editor) : Command(editor) {}

    double oldValue;
    double newValue;

    void execute() {
        document->rotation(newValue);
    }

    void undo() {
        document->rotation(oldValue);
    }
};

class BackgroundEnableCommand : public Editor::Command {
public:
    BackgroundEnableCommand(Editor *editor) : Command(editor) {}

    bool newValue;

    void execute() {
        document->backgroundEnable(newValue);
    }

    void undo() {
        document->backgroundEnable(!newValue);
    }
};


Editor::Editor(Illustrace *illustrace, Document *document)
    : illustrace(illustrace), document(document),
      _mode(Editor::Mode::Line), _lineState(Editor::LineState::Line),
      _paintState(Editor::PaintState::Brush),
      _clipState(Editor::ClipState::Trimming),
      lastCommand(nullptr),
      currentPoint(0),
      savedPoint(0)
{
}

Editor::~Editor()
{
    while (!undoStack.empty()) {
        auto *command = undoStack.top();
        undoStack.pop();
        delete command;
    }
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

void Editor::execute(Command *command)
{
    command->execute();
    if (lastCommand != command) {
        undoStack.push(command);
        ++currentPoint;
    }
    lastCommand = command;

    while (!redoStack.empty()) {
        auto *command = redoStack.top();
        redoStack.pop();
        delete command;
    }

    notify(this, Event::Execute);
}

void Editor::undo()
{
    if (canUndo()) {
        auto *command = undoStack.top();
        undoStack.pop();
        command->undo();
        redoStack.push(command);
        lastCommand = nullptr;
        --currentPoint;
        notify(this, Event::Undo);
    }
}

void Editor::redo()
{
    if (canRedo()) {
        auto *command = redoStack.top();
        redoStack.pop();
        command->execute();
        undoStack.push(command);
        lastCommand = nullptr;
        ++currentPoint;
        notify(this, Event::Redo);
    }
}

bool Editor::canUndo()
{
    return !undoStack.empty();
}

bool Editor::canRedo()
{
    return !redoStack.empty();
}

void Editor::save()
{
    lastCommand = nullptr;
    savedPoint = currentPoint;

    // TODO
    
    notify(this, Event::Save);
}

bool Editor::hasChanged()
{
    return savedPoint != currentPoint;
}

void Editor::detail(double detail)
{
    DetailCommand *command;

    if (!(command = dynamic_cast<DetailCommand *>(lastCommand))) {
        command = new DetailCommand(this);
        command->oldValue = document->detail();
    }

    command->newValue = detail;
    execute(command);
}

void Editor::thickness(double thickness)
{
    ThicknessCommand *command;

    if (!(command = dynamic_cast<ThicknessCommand *>(lastCommand))) {
        command = new ThicknessCommand(this);
        command->oldValue = document->thickness();
    }

    command->newValue = thickness;
    execute(command);
}

void Editor::rotation(double rotation)
{
    RotationCommand *command;

    if (!(command = dynamic_cast<RotationCommand *>(lastCommand))) {
        command = new RotationCommand(this);
        command->oldValue = document->rotation();
    }

    command->newValue = rotation;
    execute(command);
}

void Editor::backgroundEnable(bool enable)
{
    BackgroundEnableCommand *command;

    if (!(command = dynamic_cast<BackgroundEnableCommand *>(lastCommand))) {
        command = new BackgroundEnableCommand(this);
    }

    command->newValue = enable;
    execute(command);
}
