/**
 * Author: Codex
 * Date: 2026-10-04
 * License: CC0
 * Source: Projection onto a great circle and endpoint checks
 * Description: Shortest surface distance from p to the closed
 *  short arc ab on the unit sphere, returned in radians.
 *  a, b, p must be unit vectors; a and b must not be antipodal.
 *  Equal endpoints define a single point.
 * Usage: double d = sphereSegDist(a, b, p);
 * Time: O(1)
 * Memory: O(1)
 * Status: stress-tested
 */
#pragma once

#include "Point3D.h"

typedef Point3D<double> P3;
double sphereSegDist(P3 a, P3 b, P3 p) {
	auto dist = [](P3 x, P3 y) {
		return atan2(x.cross(y).dist(), x.dot(y));
	};
	double res = min(dist(p, a), dist(p, b));
	P3 n = a.cross(b);
	double len = n.dist();
	if (len == 0) return res;
	n = n / len;
	if (p.dot(n.cross(a)) >= 0 && p.dot(b.cross(n)) >= 0)
		res = min(res, atan2(abs(p.dot(n)), p.cross(n).dist()));
	return res;
}
