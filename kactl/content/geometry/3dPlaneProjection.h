/**
 * Author: Codex
 * Date: 2026-10-04
 * License: CC0
 * Source: Orthonormal basis and dot products
 * Description: Projects points onto the plane through origin
 *  with a finite, nonzero normal. Returns 2D coordinates in
 *  an orthonormal basis u, v, preserving input order.
 *  u cross v points along normal. Coordinates use double.
 *  Recover a projected point as origin + u*x + v*y.
 * Usage: auto r = projectToPlane(origin, normal, points);
 * Time: O(n)
 * Status: Tested with fixed cases and random invariants
 */
#pragma once

#include "Point.h"
#include "Point3D.h"

struct PlaneProjection3D {
	Point3D<double> origin, u, v;
	vector<Point<double>> points;
};

inline PlaneProjection3D projectToPlane(
		const Point3D<double>& origin,
		const Point3D<double>& normal,
		const vector<Point3D<double>>& points) {
	double scale = max({abs(normal.x), abs(normal.y),
		abs(normal.z)});
	assert(scale > 0);
	auto n = (normal / scale).unit();
	Point3D<double> ref(1, 0, 0);
	if (abs(n.y) <= abs(n.x) && abs(n.y) <= abs(n.z))
		ref = Point3D<double>(0, 1, 0);
	else if (abs(n.z) <= abs(n.x) && abs(n.z) <= abs(n.y))
		ref = Point3D<double>(0, 0, 1);
	auto u = n.cross(ref).unit(), v = n.cross(u);
	PlaneProjection3D result{origin, u, v, {}};
	result.points.reserve(points.size());
	for (const auto& p : points) {
		auto d = p - origin;
		result.points.emplace_back(d.dot(u), d.dot(v));
	}
	return result;
}
