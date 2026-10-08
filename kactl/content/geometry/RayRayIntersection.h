/**
 * Author: Codex
 * Date: 2026-10-08
 * License: CC0
 * Source: Cross products and collinear ray checks
 * Description: Checks intersection of the closed rays
 *  o1 + t*d1 and o2 + s*d2, t,s >= 0, in 2D.
 *  Requires d1 != (0,0) and d2 != (0,0).
 *  T is a signed integer type; all intermediate values must fit.
 * Usage: bool ok = rayRayInter(o1, d1, o2, d2);
 * Time: O(1)
 * Memory: O(1)
 * Status: stress-tested
 */
#pragma once
#include "Point.h"

template<class T>
bool rayRayInter(Point<T> o1, Point<T> d1,
        Point<T> o2, Point<T> d2) {
    Point<T> v = o2 - o1;
    T den = d1.cross(d2);
    if (den == 0) {
        if (v.cross(d1) != 0) return false;
        return d1.dot(d2) > 0 || v.dot(d1) >= 0;
    }
    T t = v.cross(d2), s = v.cross(d1);
    return den > 0 ? t >= 0 && s >= 0 : t <= 0 && s <= 0;
}
