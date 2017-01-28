#include "Engine/Math/IntVector3.hpp"
#include <math.h>

IntVector3::IntVector3() { }
IntVector3::IntVector3(int x, int y, int z) 
    : x(x)
	, y(y) 
	, z(z)
{}
IntVector3::IntVector3(const IntVector3& other) 
    : x(other.x)
	, y(other.y) 
	, z(other.z)
{}


float IntVector3::length() const {
	return sqrt((float)x * (float)x + (float)y * (float)y);
}

IntVector3 IntVector3::operator-() const {
	IntVector3 v;
	v.x = -x;
	v.y = -y;
	v.z = -v.z;
	return v;
}