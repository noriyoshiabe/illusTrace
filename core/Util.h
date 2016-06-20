#pragma once

namespace illustrace {
namespace util {

template<class T>
static inline T vector(const T &p1, const T &p2)
{
    return T(p2.x - p1.x, p2.y - p1.y);
}

template<class T>
static inline double dotProduct(const T &v1, const T &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

template<class T>
static inline double crossProduct(const T &v1, const T &v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

template<class T>
static inline double vectorLength(const T &p)
{
    return sqrt(p.x * p.x + p.y * p.y);
}

template<class T>
static inline double cornerRadian(const T &p1, const T &p2, const T &p3)
{
    T v1 = vector(p1, p2);
    T v2 = vector(p2, p3);
    double lv1 = vectorLength(v1);
    double lv2 = vectorLength(v2);
    double t = acos(dotProduct(v1, v2) / (lv1 * lv2));
    return std::isnan(t) ? 0.0 : t;
}

} // namespace util
} // namespace illustrace
