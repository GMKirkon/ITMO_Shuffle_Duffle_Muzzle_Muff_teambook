/**
 * Author: Codex
 * Date: 2026-10-06
 * License: CC0
 * Source: Cross products and collinear interval checks
 * Description: Checks intersection of the closed segment ab
 *  and the ray o + t*dir, t >= 0, in 2D. dir must be nonzero.
 *  Endpoint contact, collinear overlap and a == b are allowed.
 *  T is a signed integer type; all intermediate values must fit.
 * Usage: bool ok = raySegInter(a, b, o, dir);
 * Time: O(1)
 * Memory: O(1)
 * Status: stress-tested
 */
#pragma once
#include "Point.h"

template<class T>
bool raySegInter(Point<T> a, Point<T> b,
        Point<T> o, Point<T> dir) {
    Point<T> u = b - a, v = o - a;
    T den = u.cross(dir);
    if (den == 0) {
        if ((a - o).cross(dir) != 0) return false;
        return (a - o).dot(dir) >= 0 || (b - o).dot(dir) >= 0;
    }
    T t = v.cross(dir), s = v.cross(u);
    return den > 0 ? 0 <= t && t <= den && s >= 0
        : den <= t && t <= 0 && s <= 0;
}
