#pragma once

#include "Document.h"
#include "Illustrace.h"
#include "Observable.h"
#include <stack>

namespace illustrace {

class Editor : Observable<Editor> {
public:
    class Command;

    enum class Event {
        Mode,
        LineState,
        PaintState,
        ClipState,
        PreprocessedImageRadius,
        PaintLayerRadius,
        PaintColor,
        Execute,
        Undo,
        Redo,
        Save,
    };

    enum class Mode {
        Line,
        BG,
        Paint,
        Clip,
    };

    enum class LineState {
        Line,
        PencilBlack,
        PencilWhite,
        Eraser,
        Color,
    };

    enum class PaintState {
        Brush,
        Fill,
        Eraser,
        Color
    };

    enum class ClipState {
        Trimming,
        Rotate,
    };

    Editor(Illustrace *illustrace, Document *document);
    ~Editor();

    void mode(Mode mode);
    void lineState(LineState state);
    void paintState(PaintState state);
    void clipState(ClipState state);

    void radius(int radius);
    cv::Scalar &paintColor();
    
    Mode mode();
    LineState lineState();
    PaintState paintState();
    ClipState clipState();

    int radius();

    void detail(double detail);
    void thickness(double thickness);
    void rotation(double rotation);
    void backgroundEnable(bool enable);

    void draw(float x, float y);
    void drawFinish();
    void fill(float x, float y);

    void R(double red);
    void G(double green);
    void B(double blue);

    void trimmingTopLeft(float x, float y);
    void trimmingTop(float y);
    void trimmingTopRight(float x, float y);
    void trimmingRight(float x);
    void trimmingBottomRight(float x, float y);
    void trimmingBottom(float y);
    void trimmingBottomLeft(float x, float y);
    void trimmingLeft(float x);

    Illustrace *illustrace;
    Document *document;

    void undo();
    void redo();
    bool canUndo();
    bool canRedo();

    void save();
    bool hasChanged();

private:
    void execute(Command *command);
    void color(int colorIndex, double value);
    template<typename Func>
    void trimming(Func func);

    Mode _mode;
    LineState _lineState;
    PaintState _paintState;
    ClipState _clipState;
    int preprocessedImageRadius;
    int paintLayerRadius;
    cv::Scalar _paintColor;

    std::stack<Command *> undoStack;
    std::stack<Command *> redoStack;
    Command *lastCommand;

    int currentPoint;
    int savedPoint;
};

} // namespace illustrace
