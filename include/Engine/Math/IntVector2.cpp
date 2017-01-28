#include "Engine/Math/IntVector2.hpp"
#include <math.h>

IntVector2::IntVector2() { }
IntVector2::IntVector2(int x, int y) : x(x), y(y) { }
IntVector2::IntVector2(const IntVector2& other) 
    : x(other.x)
	, y(other.y) 
{ }


float IntVector2::length() const {
	return sqrt((float)x * (float)x + (float)y * (float)y);
}

IntVector2 IntVector2::operator-() const {
	IntVector2 v;
	v.x = -x;
	v.y = -y;
	return v;
}