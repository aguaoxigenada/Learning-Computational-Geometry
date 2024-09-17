#pragma once
#include "Line.h"
#include "Plane.h"

namespace jmk
{
	bool Intersection(const jmk::Point2d&, const jmk::Point2d&, const jmk::Point2d&, const jmk::Point2d&);
	bool Intersection(const jmk::Point2d&, const jmk::Point2d&, const jmk::Point2d&, const jmk::Point2d&, jmk::Point2d&);

	bool Intersection(const jmk::Line2dStd&, const jmk::Line2dStd&, jmk::Point2d&);

	bool Intersection(const jmk::Line3dStd& line, const jmk::Planef& plane, jmk::Point3d& point);

	bool intersect(const jmk::Planef& p1, const jmk::Planef& p2, jmk::Line& l);

}
