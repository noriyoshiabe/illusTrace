#pragma once

namespace illustrace {
namespace core {

template <class T>
struct BezierVertex {
    T pt;

    struct {
        T next;
        T prev;
    } ctl;

    BezierVertex(T pt, T ctlNext, T ctlPrev) : pt(pt), ctl({ctlNext, ctlPrev}) {};
};

} // namespace core
} // namespace illustrace
