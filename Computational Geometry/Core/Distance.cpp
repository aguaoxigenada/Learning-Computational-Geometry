#include "Distance.h"

/*
float jmk::distance(Line3dStd& line, Point3d& C)
{
	auto AC = C - line.getPoint();
	auto t = dotProduct(line.getDir(), AC);

	auto xt = line.getPoint() + line.getDir() * t;
	auto dist_vec = xt - C;
	return dist_vec.magnitude();
}

*/

float jmk::distance(Line& line, Point3d& C)
{
	// TODO we can simple call distance frunction which takes three points here as well.
	// But then the methodology of finding the distance is not visible to students. So keep this.
	// Ignor the fact that we duplicate the similar implementation, which is not the best practise.
	Vector3f AC = C - line.point();
	auto t = dotProduct(line.direction(), AC);

	auto xt = line.point() + line.direction() * t;
	auto dist_vec = xt - C;

	return dist_vec.magnitude();
}

// Podria intentar de crear la otra formula de distancia para el punto y linea luego.


float jmk::distance(Planef& p, Point3d& Q)
{
	auto result = dotProduct(p.getNormal(), Q) - p.getD();
	return result;
}
