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
        ShapeState,
        PaintState,
        ClipState,
        PreprocessedImageThickness,
        PaintLayerThickness,
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
        CASE(ShapeState);
        CASE(PaintState);
        CASE(ClipState);
        CASE(PreprocessedImageThickness);
        CASE(PaintLayerThickness);
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

    enum class ShapeState {
        Line,
        Pencil,
        Eraser,
        Color,
    };

    static inline const char *ShapeState2CString(ShapeState state) {
#define CASE(state) case ShapeState::state: return #state
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
    void shapeState(ShapeState state);
    void paintState(PaintState state);
    void clipState(ClipState state);

    void drawThickness(int drawThickness);
    cv::Scalar &paintColor();
    
    Mode mode();
    ShapeState shapeState();
    PaintState paintState();
    ClipState clipState();

    int drawThickness();

    void detail(double detail);
    void thickness(double thickness);
    void rotation(double rotation);
    void backgroundEnable(bool enable);

    void draw(float x, float y);
    void drawFinish();
    void reload();
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
    ShapeState _shapeState;
    PaintState _paintState;
    ClipState _clipState;
    int preprocessedImageThickness;
    int paintLayerThickness;
    cv::Scalar _paintColor;

    std::stack<Command *> undoStack;
    std::stack<Command *> redoStack;
    Command *lastCommand;

    int currentPoint;
    int savedPoint;
};

} // namespace illustrace
