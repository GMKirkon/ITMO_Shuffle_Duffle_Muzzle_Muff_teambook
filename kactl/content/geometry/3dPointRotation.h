/**
 * Author: Codex
 * Date: 2026-10-04
 * License: CC0
 * Source: Point3D::rotate
 * Description: Rotates p around the line through distinct
 *  points a and b by alpha radians. Positive angles follow
 *  the right-hand rule along a -> b. Uses double coordinates.
 * Usage: auto q = rotateAroundLine(p, a, b, alpha);
 * Time: O(1)
 * Status: Tested with fixed cases and random invariants
 */
#pragma once

#include "Point3D.h"

inline Point3D<double> rotateAroundLine(
		const Point3D<double>& p, const Point3D<double>& a,
		const Point3D<double>& b, double alpha) {
	assert(!(a == b));
	return a + (p - a).rotate(alpha, b - a);
}
