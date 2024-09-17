#pragma once

#include <vector>

#include "Primitives/Line.h"
#include "Primitives\Point.h"

#include "Primitives\Polygon.h"


namespace  jmk
{
	// Return the area of the triangle defined by given 3 points in XY 2D space
	double areaTriangle2d(const Point2d& a, const Point2d& b, const Point2d& c);

	// Return the area of the triangle defined by given 3 points
	double areaTriangle3d(const Point3d& a, const Point3d& b, const Point3d& c);

	int orientation2d(const Point2d& a, const Point2d& b, const Point2d& c);

	int orientation3d(const Point3d& a, const Point3d& b, const Point3d& c);

	bool collinear(const Vector3f& a, const Vector3f& b);

	bool collinear(const Point3d& a, const Point3d& b, const Point3d& c);

	bool coplanar(const Point3d& a, const Point3d& b, const Point3d& c, const Point3d& d);

	bool coplanar(const Vector3f& a, const Vector3f& b, const Vector3f& c);

	// Predicate to determine whether the [Point c] is left to the segment [a b]
	bool left(const Point3d& a, const Point3d& b, const Point3d& c);

	// Predicate to determine whether the [Point c] is left to the segment [a b]
	bool left(const Point2d& a, const Point2d& b, const Point2d& c);

	//bool left(const Line2dStd& l, const Point2d& p);

	bool left(const Line2d& l, const Point2d& p);

	// Predicate to determine whether the [Point c] is right to the segment [a b]
	bool right(const Point3d& a, const Point3d& b, const Point3d& c);

	// Predicate to determine whether the[Point c] is left to the segment[a b]
	bool leftOrBeyond(const Point2d& a, const Point2d& b, const Point2d& c);

	// Predicate to determine whether the [Point c] is left to the segment [a b]
	bool leftOrBeyond(const Point3d& a, const Point3d& b, const Point3d& c);

	// Predicate to determine whether the [Point c] is left to or between the segment [a b]
	bool leftOrBetween(const Point3d& a, const Point3d& b, const Point3d& c);

	bool isDiagonal(const Vertex2dSimple* v1, const Vertex2dSimple* v2, Polygon2dSimple* poly = nullptr);
}
