#include "Vector.h"




float jmk::crossProduct2D(jmk::Vector2f v1, jmk::Vector2f v2)
{
	return v1[X] * v2[Y] - v1[Y] * v2[X];
}

jmk::Vector3f jmk::crossProduct3D(jmk::Vector3f v1, jmk::Vector3f v2)
{
	float x_, y_, z_;
	x_ = v1[Y] * v2[Z] - v2[Y] * v1[Z];
	y_ = -(v2[Z] * v1[X] - v1[Z] * v2[X]);  // es lo mimso y_ = v1[Z] * v2[X] - v1[X] * v2[Z];
	z_ = v1[X] * v2[Y] - v2[X] * v1[Y];

	return Vector3f(x_, y_, z_);
}

float jmk::scalerTripleProduct(Vector3f v1, Vector3f v2, Vector3f v3)
{
	auto bc_cross = crossProduct3D(v2, v3);

	return dotProduct(v1, bc_cross);
}

