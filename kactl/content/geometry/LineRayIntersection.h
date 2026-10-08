/**
 * Author: Codex
 * Date: 2026-10-08
 * License: CC0
 * Source: Signs of cross products
 * Description: Checks intersection of the line through a, b
 *  and the closed ray o + t*dir, t >= 0, in 2D.
 *  Requires a != b and dir != (0,0).
 *  T is a signed integer type; all intermediate values must fit.
 * Usage: bool ok = lineRayInter(a, b, o, dir);
 * Time: O(1)
 * Memory: O(1)
 * Status: stress-tested
 */
#pragma once
#include "Point.h"

template<class T>
bool lineRayInter(Point<T> a, Point<T> b,
        Point<T> o, Point<T> dir) {
    Point<T> u = b - a;
    T side = u.cross(o - a), step = u.cross(dir);
    return side == 0 || (side < 0 && step > 0)
        || (side > 0 && step < 0);
}
