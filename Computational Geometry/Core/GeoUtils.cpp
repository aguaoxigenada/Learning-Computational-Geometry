#include "GeoUtils.h"

#include "Base/Vector.h"
#include "Primitives/Intersection.h"


using namespace jmk;

double jmk::areaTriangle2d(const Point2d& a, const Point2d& b, const Point2d& c)
{
	auto AB = b - a;
	auto AC = c - a;
	auto result = crossProduct2D(AB, AC);
	return result / 2;

}

double jmk::areaTriangle3d(const Point3d& a, const Point3d& b, const Point3d& c)
{
	float x_, y_, z_;

	Vector3f AB = b - a;
	Vector3f AC = c - a;

	x_ = AB[Y] * AC[Z] - AB[Z] * AC[Y];
	y_ = AB[X] * AC[Z] - AB[Z] * AC[X];
	z_ = AB[X] * AC[Y] - AB[Y] * AC[X];

	float sum_of_powers = pow(x_, 2.0) + pow(y_, 2.0) + pow(z_, 2.0);
	float root = sqrtf(sum_of_powers);
	return root / 2;
}


int jmk::orientation2d(const Point2d& a, const Point2d& b, const Point2d& c)
{
	auto area = areaTriangle2d(a, b, c);

	if (area > 0 && area < TOLERANCE)
		area = 0;

	if (area < 0 && area > TOLERANCE)
		area = 0;

	Vector2f ab = b - a;
	Vector2f ac = c - a;

	if (area > 0.0)
		return LEFT;
	if (area < 0.0)
		return RIGHT;
	if ((ab[X] * ac[X] < 0.0) || (ab[Y] * ac[Y] < 0.0))
		return BEHIND;
	if (ab.magnitude() < ac.magnitude())
		return BEYOND;
	if (a == c)
		return ORIGIN;
	if (b == c)
		return DESTINATION;
	return BETWEEN;

}


int jmk::orientation3d(const Point3d& a, const Point3d& b, const Point3d& c)
{
	float area = areaTriangle3d(a, b, c);

	if (area > 0 && area < TOLERANCE)
		area = 0;

	if (area < 0 && area > TOLERANCE)
		area = 0;

	Point3d p1 = b - a;
	Point3d p2 = c - a;

	double p1x, p1y, p2x, p2y;

	p1x = p1[X];
	p1y = p1[Y];
	p2x = p2[X];
	p2y = p2[Y];

	if (area > 0.0)
		return LEFT;
	if (area < 0.0)
		return RIGHT;
	if ((p1x * p2x < 0.0) || (p1y * p2y < 0.0))
		return BEHIND;
	if (p1.magnitude() < p2.magnitude())
		return BEYOND;
	if (a == c)
		return ORIGIN;
	if (b == c)
		return DESTINATION;
	return BETWEEN;

	return 0;
}


bool jmk::collinear(const Vector3f& a, const Vector3f& b)
{
	auto v1 = a[X] * b[Y] - a[Y] * b[X];
	auto v2 = a[Y] * b[Z] - a[Z] * b[Y];
	auto v3 = a[X] * b[Z] - a[Z] * b[X];

	return isEqualD(v1, ZERO) && isEqualD(v2, ZERO) && isEqualD(v3, ZERO);
}
bool jmk::collinear(const Point3d& a, const Point3d& b, const Point3d& c)
{
	auto AB = b - a;
	auto AC = c - a;

	return collinear(AB, AC);
}

bool jmk::coplanar(const Point3d& a, const Point3d& b, const Point3d& c, const Point3d& d)
{
	auto AB = b - a;
	auto AC = c - a;
	auto AD = d - a;
	return coplanar(AB, AC, AD);
}

bool jmk::coplanar(const Vector3f& a, const Vector3f& b, const Vector3f& c)
{
	float value = scalerTripleProduct(a, b, c);
	return isEqualD(value, ZERO);  // El 1/6 de la formula del tetrahedron ya no es necesario si el scalartripleproduct es igual a 0.
}


bool jmk::left(const Point3d& a, const Point3d& b, const Point3d& c)
{
	return orientation3d(a, b, c) == RELATIVE_POSITION::LEFT;
}


bool jmk::left(const Point2d& a, const Point2d& b, const Point2d& c)
{
	return orientation2d(a, b, c) == RELATIVE_POSITION::LEFT;
}

/*
bool jmk::left(const Line2dStd& l, const Point2d& p)
{
	auto line_dir = l.getDir();
	Vector2f line_normal(-line_dir[Y], line_dir[X]);
	auto value = dotProduct(line_normal, p);
	return (dotProduct(line_normal, p) - l.getD()) < 0 ? false : true;
}
*/

bool jmk::left(const Line2d& l, const Point2d& p)
{
	auto line_normal = l.normal();
	auto value = dotProduct(line_normal, p);
	auto d = dotProduct(line_normal, l.point());
	return (dotProduct(line_normal, p) - d) < 0 ? false : true;
}

bool jmk::right(const Point3d& a, const Point3d& b, const Point3d& c)
{
	return orientation3d(a, b, c) == RELATIVE_POSITION::RIGHT;
}

bool jmk::leftOrBeyond(const Point2d& a, const Point2d& b, const Point2d& c)
{
	int position = orientation2d(a, b, c);
	return (position == RELATIVE_POSITION::LEFT || position == RELATIVE_POSITION::BEYOND);
}

bool jmk::leftOrBeyond(const Point3d& a, const Point3d& b, const Point3d& c)
{
	int position = orientation3d(a, b, c);
	return (position == RELATIVE_POSITION::LEFT || position == RELATIVE_POSITION::BEYOND);
}

bool jmk::leftOrBetween(const Point3d& a, const Point3d& b, const Point3d& c)
{
	int position = orientation3d(a, b, c);
	return (position == RELATIVE_POSITION::LEFT || position == RELATIVE_POSITION::BETWEEN);
}

static bool interiorCheck(const Vertex2dSimple* v1, const Vertex2dSimple* v2)
{
	if (jmk::leftOrBeyond(v1->point, v1->next->point, v1->prev->point))
	{
		// v1 is convex vertex
		return jmk::left(v1->point, v2->point, v1->prev->point)
			&& jmk::left(v2->point, v1->point, v1->next->point);
	}

	// v1 is reflex vertex
	return !(jmk::leftOrBeyond(v1->point, v2->point, v1->next->point) 
		&& jmk::leftOrBeyond(v2->point, v1->point, v1->prev->point));
}

bool jmk::isDiagonal(const Vertex2dSimple* v1, const Vertex2dSimple* v2, Polygon2dSimple* poly)
{
	bool prospect = true;
	std::vector<Vertex2dSimple*> vertices;

	if (poly)
		vertices = poly->getVertices();
	else
	{
		auto vertex_ptr = v1->next;
		vertices.push_back((Vertex2dSimple*)v1);
		while (vertex_ptr != v1) 
		{
			vertices.push_back(vertex_ptr);
			vertex_ptr = vertex_ptr->next;
		}
	}

	Vertex2dSimple* current, * next;
	current = vertices[0];

	do
	{
		next = current->next;
		if (current != v1 && next != v1 && current != v2 && next != v2
			&& jmk::Intersection(v1->point, v2->point, current->point, next->point))
		{
			prospect = false;
			break;
		}
		current = next;
	}	while (current != vertices[0]);


	return prospect && interiorCheck(v1, v2) && interiorCheck(v2, v1);
}



