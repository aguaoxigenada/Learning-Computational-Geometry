#include "../Base/Core.h"
#include "Intersection.h"
#include "../GeoUtils.h"

using namespace jmk;

bool jmk::Intersection(const jmk::Point2d& a, const jmk::Point2d& b, const jmk::Point2d& c, const jmk::Point2d& d)
{

	auto ab_c = jmk::orientation2d(a, b, c);
	auto ab_d = jmk::orientation2d(a, b, d);
	auto cd_a = jmk::orientation2d(c, d, a);
	auto cd_b = jmk::orientation2d(c, d, b);

	if (ab_c == BETWEEN || ab_c == ORIGIN || ab_c == DESTINATION
		|| ab_d == BETWEEN || ab_d == ORIGIN || ab_d == DESTINATION
		|| cd_a == BETWEEN || cd_a == ORIGIN || cd_a == DESTINATION
		|| cd_b == BETWEEN || cd_b == ORIGIN || cd_b == DESTINATION)
	{
		return true;
	}


	return _xor(ab_c == LEFT, ab_d == LEFT) && _xor(cd_a == LEFT, cd_b == LEFT);
}


// Function to calculate the intersection of two line segments defined by points a to b and c to d.
// If the segments intersect, the intersection point is stored in the 'intersection' variable and the function returns true.
// If the segments do not intersect, the function returns false.
bool jmk::Intersection(const jmk::Point2d& a, const jmk::Point2d& b, const jmk::Point2d& c, const jmk::Point2d& d, jmk::Point2d& intersection)
{
    // Calculate the direction vectors of the line segments AB and CD
    Vector2f AB = b - a;
    Vector2f CD = d - c;

    // Calculate the normal vector to CD. This is a vector perpendicular to CD. (counterclockwise)
    Vector2f n(CD[Y], -CD[X]);
    // siempre funciona esto, se puede probar en geoalgebra.

    // Calculate the dot product of the normal vector n and the direction vector AB
    // This value will be used to determine if the lines are parallel.
    auto deno = dotProduct(n, AB);

    // Check if the denominator is zero, which means the lines are parallel and there is no intersection
    if (!isEqualD(deno, ZERO))
    {
        // The lines are not parallel, calculate the vector AC from point a to point c
        auto AC = c - a;

        // Calculate the numerator using the dot product of the normal vector n and the vector AC
        auto numerator = dotProduct(n, AC);

        // Calculate the parameter t, which is used to find the intersection point
        auto t = numerator / deno;

        // Calculate the x and y coordinates of the intersection point using the parameter t
        auto x = a[X] + t * AB[X];
        auto y = a[Y] + t * AB[Y];


        // si se quisiera se podria usar el CD vector para el mismo fin


        // Assign the calculated coordinates to the intersection point
        intersection.assign(X, x);
        intersection.assign(Y, y);

        // Return true indicating that the intersection point is found
        return true;
    }
    else
    {
        // The lines are parallel, return false indicating no intersection
        return false;
    }
}

// Function to calculate the intersection of two lines defined by jmk::Line2d objects l1 and l2.
// If the lines intersect, the intersection point is stored in the 'intersection' variable and the function returns true.
// If the lines do not intersect, the function returns false.
bool jmk::Intersection(const jmk::Line2dStd& l1, const jmk::Line2dStd& l2, jmk::Point2d& intersection)
{
    // Get the starting point of the first line
    auto l1_start = l1.getPoint();
		
    // Calculate the end point of the first line using its direction
    auto l1_end = l1_start + l1.getDir();


    // r(t) = r + td  r = starting point / d = direction vector  / d = scalar.
    // se asume que d es igual 1, porque ya completo hasta el final, por eso al end point le sumas el start, para sacar t = 1.
    // dado que la direccion esta normalizada en la clase de linea.

    // Get the starting point of the second line
    auto l2_start = l2.getPoint();

    // Calculate the end point of the second line using its direction
    auto l2_end = l2_start + l2.getDir();

    // Use the first Intersection function to find the intersection point of the line segments
    return Intersection(l1_start, l1_end, l2_start, l2_end, intersection);
}

bool jmk::Intersection(const jmk::Line3dStd& line, const jmk::Planef& plane, jmk::Point3d& point)
{
    auto n = plane.getNormal();
    auto D = plane.getD();
    auto d = line.getDir();
    auto p = line.getPoint();

    auto nd = dotProduct(n, d);

    if(!isEqualD(nd, ZERO))
    {
        auto t = -1 * dotProduct(n, p) + D / nd;
        point.assign(X, p[X] + t * d[X]);
        point.assign(Y, p[Y] + t * d[Y]);
        point.assign(Z, p[Z] + t * d[Z]);
        return true;
    }
    else
    {
        return false;
    }
}

bool jmk::intersect(const jmk::Planef& p1, const jmk::Planef& p2, jmk::Line& l)
{
    auto n1 = p1.getNormal();
    auto n2 = p2.getNormal();

    auto d1 = p1.getD();
    auto d2 = p2.getD();

    auto direction = crossProduct3D(n1, n2);

    // Check if the planes are parallel.
    if (isEqualD(direction.magnitude(), ZERO))
        return false;

    auto n1n2 = dotProduct(n1, n2);
    auto n1n2_2 = n1n2 * n1n2;

    auto a = (d2 * n1n2 - d1) / (n1n2_2 - 1);
    auto b = (d1 * n1n2 - d2) / (n1n2_2 - 1);

    auto point = n1 * a + n2 * b;

    l.setPoint(point);
    direction.normalize();
    l.setDirection(direction);

    return true;

}


