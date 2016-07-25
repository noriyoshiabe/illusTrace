#pragma once

#include "Document.h"
#include "Illustrace.h"
#include "Observable.h"
#include <stack>

namespace illustrace {

class Editor : public Observable<Editor> {
public:
    class Command {
    public:
        Command(Editor *editor) : document(editor->document), illustrace(editor->illustrace) {};
        virtual ~Command() {}
        virtual void execute() = 0;
        virtual void undo() = 0;
        virtual void redo() { execute(); }

        Document *document;
        Illustrace *illustrace;
    };

    enum class Event : int {
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

    static inline const char *Event2CString(Event event)
    {
#define CASE(event) case Event::event: return #event
        switch (event) {
        CASE(Mode);
        CASE(LineState);
        CASE(PaintState);
        CASE(ClipState);
        CASE(PreprocessedImageRadius);
        CASE(PaintLayerRadius);
        CASE(PaintColor);
        CASE(Execute);
        CASE(Undo);
        CASE(Redo);
        CASE(Save);
        }
#undef CASE
    }

    enum class Mode {
        Shape,
        BG,
        Paint,
        Clip,
    };

    static inline const char *Mode2CString(Mode mode) {
#define CASE(mode) case Mode::mode: return #mode
        switch (mode) {
        CASE(Shape);
        CASE(BG);
        CASE(Paint);
        CASE(Clip);
        }
#undef CASE
    }

    enum class LineState {
        Line,
        Pencil,
        Eraser,
        Color,
    };

    static inline const char *LineState2CString(LineState state) {
#define CASE(state) case LineState::state: return #state
        switch (state) {
        CASE(Line);
        CASE(Pencil);
        CASE(Eraser);
        CASE(Color);
        }
#undef CASE
    }

    enum class PaintState {
        Brush,
        Fill,
        Eraser,
        Color
    };

    static inline const char *PaintState2CString(PaintState state) {
#define CASE(state) case PaintState::state: return #state
        switch (state) {
        CASE(Brush);
        CASE(Fill);
        CASE(Eraser);
        CASE(Color);
        }
#undef CASE
    }

    enum class ClipState {
        Trimming,
        Rotate,
    };

    static inline const char *ClipState2CString(ClipState state) {
#define CASE(state) case ClipState::state: return #state
        switch (state) {
        CASE(Trimming);
        CASE(Rotate);
        }
#undef CASE
    }

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

    friend std::ostream &operator<<(std::ostream &os, Editor const &self);

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
