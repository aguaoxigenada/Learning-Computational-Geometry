#pragma once

#include "../Base/Vector.h"
#include "Point.h"

namespace jmk
{
	template<class coord_type>
	class Plane
	{

		Vector3f normal;
		float d = 0;

	public:
		Plane(){}

		Plane(Vector3f& _normal, float _constant):normal(_normal), d(_constant)
		{
			normal.normalize();
		}

		Plane(Point3d& _p1, Point3d& _p2, Point3d& _p3 )
		{
			auto v1 = _p2 - _p1;
			auto v2 = _p3 - _p1;

			normal = crossProduct3D(v1, v2);
			normal.normalize();
			d = dotProduct(normal, _p1);
		}

		Vector3f getNormal() const
		{
			return normal;
		}

		float getD() const
		{
			return d;
		}
	};

	typedef  Plane<float> Planef;
}
