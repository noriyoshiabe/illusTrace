#pragma once

namespace illustrace {

template <class T>
struct BezierVertex {
    T pt;

    struct {
        T next;
        T prev;
    } ctl;

    BezierVertex(T pt, T ctlNext, T ctlPrev) : pt(pt), ctl({ctlNext, ctlPrev}) {};
};

} // namespace illustrace
