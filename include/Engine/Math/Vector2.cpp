#include "Engine/Math/Vector2.hpp"

#define STATIC

STATIC const Vector2 Vector2::UNIT_VECTOR_X(1.f, 0.f);
STATIC const Vector2 Vector2::UNIT_VECTOR_Y(0.f, 1.f);
STATIC const Vector2 Vector2::ZERO(0.f, 0.f);
STATIC const Vector2 Vector2::ONE(1.f, 1.f);

STATIC const Vector2 Vector2::TEXCOORD_TOP_LEFT(0.f, 0.f);
STATIC const Vector2 Vector2::TEXCOORD_TOP_RIGHT(1.f, 0.f);
STATIC const Vector2 Vector2::TEXCOORD_BOTTOM_RIGHT(1.f, 1.f);
STATIC const Vector2 Vector2::TEXCOORD_BOTTOM_LEFT(0.f, 1.f);


//STRUCTORS
Vector2::Vector2() {}
Vector2::Vector2(float x, float y)
	: x(x)
	, y(y)
{}
Vector2::Vector2(const Vector2& other) : 
	x(other.x), 
	y(other.y) 
{}

float Vector2::length() const {
	return sqrt(x * x + y * y);
}
void Vector2::normalize() {
	float length = sqrt(x * x + y * y);
	x = x / length;
	y = y / length;
}

Vector2 Vector2::operator-() const {
	Vector2 v;
	v.x = -x;
	v.y = -y;
	return v;
}