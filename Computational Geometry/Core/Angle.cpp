#include "Angle.h"
#include <cmath>

using namespace jmk;

template<class T, size_t dim>
static float getAngle(jmk::Vector<T, dim> v1, jmk::Vector<T,dim>v2)
{
	auto dot = dotProduct(v1, v2);
	auto theta = acos(fabs(dot));   // debe estar llegando normalizado los vectores por eso no se necesita la magnitud. si es asi
	return RadiansToDegrees(theta);
}

float jmk::AngleLines2D( const Line2d& l1, const Line2d l2)
{
	return getAngle(l1.direction(), l2.direction());
}

float jmk::AngleLines3D(const Line& l1, const Line& l2)
{
	return getAngle(l1.direction(), l2.direction());
}


float jmk::AngleLinePlane(const Line& l, const Planef p)
{
	auto theta = getAngle(l.direction(), p.getNormal());
	return 90 - theta;
}


float jmk::AnglePlanes(const Planef p1, const Planef p2)
{
	return getAngle(p1.getNormal(), p2.getNormal());
}
