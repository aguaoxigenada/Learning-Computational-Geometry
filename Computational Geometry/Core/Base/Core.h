#pragma once

#define _USE_MATH_DEFINES

#include<math.h>

namespace jmk {
#define TOLERANCE 0.0000000001

#define ZERO 0

	enum RELATIVE_POSITION
	{
		LEFT, RIGHT, BEHIND, BEYOND, BETWEEN, ORIGIN, DESTINATION
	};

	static bool isEqualD(double x, double y)
	{
		return fabs(x - y) < TOLERANCE;
	}

	static bool _xor(bool x, bool y)
	{
		return x ^ y;
	}

	static float RadiansToDegrees(float radians)
	{
		return radians * 360 / (2 * M_PI);
	}
}
