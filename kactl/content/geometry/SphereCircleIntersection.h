/**
 * Author: Codex
 * Date: 2026-10-04
 * License: CC0
 * Source: Intersection of two planes with the unit sphere
 * Description: Intersects spherical circles with unit centers
 *  a, b and angular radii r1, r2 in [0, pi], in radians.
 *  Returns the number of intersections: 0, 1, or 2.
 *  Returns -1 for coincident nondegenerate circles.
 *  For 1, out->first = out->second. For 0 or -1, out is unchanged.
 *  Radii 0 and pi define single points. eps is angular tolerance.
 * Usage: pair<P3, P3> out; int cnt = sphereCircleInter(a,b,r1,r2,&out);
 * Time: O(1)
 * Memory: O(1)
 * Status: stress-tested
 */
#pragma once

#include "Point3D.h"

typedef Point3D<double> P3;
int sphereCircleInter(P3 a, P3 b, double r1, double r2,
		pair<P3, P3>* out, double eps = 1e-9) {
	double pi = acos(-1.0);
	if (r1 <= eps || pi - r1 <= eps) {
		P3 p = a * (r1 <= eps ? 1.0 : -1.0);
		double d = atan2(p.cross(b).dist(), p.dot(b));
		if (abs(d - r2) > eps) return 0;
		*out = {p, p};
		return 1;
	}
	if (r2 <= eps || pi - r2 <= eps)
		return sphereCircleInter(b, a, r2, r1, out, eps);
	P3 n = a.cross(b);
	double s = n.dist(), t = a.dot(b);
	if (s <= eps)
		return abs(r1 - (t > 0 ? r2 : pi - r2)) <= eps ? -1 : 0;
	double d = atan2(s, t), lo = abs(r1 - r2);
	double hi = min(r1 + r2, 2*pi - r1 - r2);
	if (d < lo - eps || d > hi + eps) return 0;
	n = n / s;
	double c = cos(r1), w = sin(d / 2);
	double y = (2*c*w*w -
		2*sin((r1+r2)/2)*sin((r2-r1)/2)) / s;
	P3 mid = a*c + n.cross(a)*y;
	if (d <= lo + eps || d >= hi - eps) {
		mid = mid.unit();
		*out = {mid, mid};
		return 1;
	}
	double sr = sin(r1);
	P3 per = n * sqrt(fmax(0.0, sr*sr - y*y));
	*out = {mid + per, mid - per};
	return 2;
}
