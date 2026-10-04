/**
 * Author: Victor Lecomte, chilli, Codex
 * Date: 2026-10-04
 * License: CC0
 * Source: https://vlecomte.github.io/cp-geo.pdf
 * Description: Returns -1 inside a simple polygon, 0 on
 *  its boundary, and 1 outside. Collinear consecutive
 *  vertices are allowed. Assumes exact arithmetic in T
 *  and that all intermediate results fit.
 * Usage: int r = pointInPolygon(poly, p);
 * Time: O(n)
 * Memory: O(1)
 * Status: Stress-tested on 100000 random point queries
 */
#pragma once

#include "Point.h"
#include "OnSegment.h"

template<class T>
int pointInPolygon(const vector<Point<T>>& poly, Point<T> p) {
	bool inside = false;
	for (size_t i = 0; i < poly.size(); ++i) {
		auto a = poly[i], b = poly[(i + 1) % poly.size()];
		if (onSegment(a, b, p)) return 0;
		int side = sgn(a.cross(b, p));
		if ((a.y <= p.y && p.y < b.y && side > 0)
				|| (b.y <= p.y && p.y < a.y && side < 0))
			inside = !inside;
	}
	return inside ? -1 : 1;
}

template<class T>
bool inPolygon(const vector<Point<T>>& poly, Point<T> p,
		bool strict = true) {
	int r = pointInPolygon(poly, p);
	return strict ? r == -1 : r <= 0;
}
