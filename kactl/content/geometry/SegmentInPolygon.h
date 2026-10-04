/**
 * Author: Codex
 * Date: 2026-10-04
 * License: CC0
 * Source: Boundary intersections and vertex angles
 * Description: Returns true iff the whole segment lies
 *  in a simple polygon, including its boundary. Collinear
 *  consecutive vertices and either orientation are allowed.
 *  Assumes exact arithmetic in T and that all results fit.
 * Usage: bool ok = segmentInPolygon(poly, a, b);
 * Time: O(n)
 * Memory: O(n)
 * Status: Stress-tested against an exact rational oracle
 */
#pragma once

#include "InsidePolygon.h"

template<class T>
bool segmentInPolygon(const vector<Point<T>>& poly,
		Point<T> a, Point<T> b) {
	if (pointInPolygon(poly, a) > 0
			|| pointInPolygon(poly, b) > 0) return false;
	if (a == b) return true;
	auto side = [&](Point<T> x, Point<T> y, Point<T> z) {
		return sgn(x.cross(y, z));
	};
	vector<Point<T>> v;
	for (auto p : poly)
		if (v.empty() || !(p == v.back())) v.push_back(p);
	if (v.size() > 1 && v.front() == v.back()) v.pop_back();
	int n = (int)v.size();
	if (n < 3) return false;
	int k = min_element(v.begin(), v.end()) - v.begin();
	int dir = side(v[(k + n - 1) % n], v[k],
		v[(k + 1) % n]);
	if (!dir) return false;
	auto inAngle = [&](Point<T> prev, Point<T> cur,
			Point<T> next, Point<T> p) {
		int turn = side(prev, cur, next) * dir;
		int s = side(prev, cur, p) * dir;
		int t = side(cur, next, p) * dir;
		return turn < 0 ? s >= 0 || t >= 0
			: s >= 0 && t >= 0;
	};
	for (int i = 0; i < n; ++i) {
		auto p = v[i], q = v[(i + 1) % n];
		int s = side(a, b, p), t = side(a, b, q);
		int u = side(p, q, a), w = side(p, q, b);
		if (s * t < 0 && u * w < 0) return false;
		if (onSegment(p, q, a) && !(a == p) && !(a == q)
				&& w * dir < 0) return false;
		if (onSegment(p, q, b) && !(b == p) && !(b == q)
				&& u * dir < 0) return false;
		if (onSegment(a, b, p)) {
			auto prev = v[(i + n - 1) % n];
			if (!inAngle(prev, p, q, a)
					|| !inAngle(prev, p, q, b)) return false;
		}
	}
	return true;
}
