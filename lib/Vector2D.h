#pragma once

namespace illustrace {
namespace lib {

template<class T>
inline T vector(const T &p1, const T &p2)
{
    return T(p2.x - p1.x, p2.y - p1.y);
}

template<class T>
inline double dotProduct(const T &v1, const T &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

template<class T>
inline double crossProduct(const T &v1, const T &v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

template<class T>
inline double vectorLength(const T &p)
{
    return sqrt(p.x * p.x + p.y * p.y);
}

} // namespace lib
} // namespace illustrace
