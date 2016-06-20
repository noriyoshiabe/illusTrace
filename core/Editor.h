#pragma once

#include "Document.h"
#include "Illustrace.h"

#include <stack>

namespace illustrace {

class Operation {
public:
    virtual ~Operation() {};
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class Editor {
public:
    Editor(Illustrace &illustrace, Document &document);
    ~Editor();

    void mode(LineMode mode);
    void brightness(double brightness);
    void detail(double detail);
    void smoothing(double smoothing);
    void thickness(double thickness);
    void scale(double scale);
    void rotation(double rotation);
    void correctionLayer(cv::Mat &layer);
    void clippingRect(cv::Rect &rect);

    void undo();
    void redo();

    bool canUndo();
    bool canRedo();

private:
    std::stack<Operation> undoStack;
    std::stack<Operation> redoStack;
};

} // namespace illustrace
