#pragma once

#include <math.h>
#include "Engine/ErrorWarningAssert.hpp"

class Vector2 {
public:
	//STRUCTORS
	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& other);

	float length() const;
	void normalize();

	inline Vector2 operator-() const;

	//OPERATOR OVERLOADS
	inline friend Vector2 operator+(const Vector2 &p1, const Vector2 &p2);
	inline friend Vector2 operator-(const Vector2 &p1, const Vector2 &p2);

	inline Vector2 Vector2::operator+=(const Vector2 &p);
	inline Vector2 Vector2::operator-=(const Vector2 &p);
	inline Vector2 Vector2::operator+=(const int scalar);
	inline Vector2 Vector2::operator-=(const int scalar);

	inline friend Vector2 operator+(const Vector2 &p1, const float scalar);
	inline friend Vector2 operator-(const Vector2 &p1, const float scalar);
	inline friend Vector2 operator+(const float scalar, const Vector2 &p1);
	inline friend Vector2 operator-(const float scalar, const Vector2 &p1);

	inline friend Vector2 operator*(const Vector2& v1, const Vector2& v2);
	inline friend Vector2 operator*(const Vector2& v, const float scalar);
	inline friend Vector2 operator*(const float scalar, const Vector2& v);
	inline friend Vector2 operator/(const Vector2& v, const float scalar);


	//CONDITIONAL OPERATORS
	bool operator<(const Vector2 &other) const {
		if (this->x <= other.x && this->y < other.y)
			return true;
		else if (this->x < other.x && this->y <= other.y)
			return true;
		else
			return false;
	}
	bool operator>(const Vector2 &other) const {
		return !(*this < other);
	}
	bool operator<=(const Vector2 &other) const {
		if (this->x <= other.x && this->y <= other.y)
			return true;
		else
			return false;
	}
	bool operator>=(const Vector2 &other) const {
		if (this->x >= other.x && this->y >= other.y)
			return true;
		else
			return false;
	}
	bool operator==(const Vector2 &other) const {
		if (this->x == other.x && this->y == other.y)
			return true;
		else
			return false;
	}
	bool operator!=(const Vector2 &other) const {
		return !(*this == other);
	}

public:
	static const Vector2 UNIT_VECTOR_X;
	static const Vector2 UNIT_VECTOR_Y;
	static const Vector2 ZERO;
	static const Vector2 ONE;
	
	static const Vector2 TEXCOORD_TOP_LEFT;
	static const Vector2 TEXCOORD_TOP_RIGHT;
	static const Vector2 TEXCOORD_BOTTOM_RIGHT;
	static const Vector2 TEXCOORD_BOTTOM_LEFT;


public:
	float x;
	float y;
};

inline Vector2 operator+(const Vector2 &p1, const Vector2 &p2) {
	return Vector2(p1.x + p2.x, p1.y + p2.y);
}
inline Vector2 operator-(const Vector2 &p1, const Vector2 &p2) {
	return Vector2(p1.x - p2.x, p1.y - p2.y);
}

inline Vector2 Vector2::operator+=(const Vector2 &p) {
	return Vector2(x + p.x, y + p.y);
}
inline Vector2 Vector2::operator-=(const Vector2 &p) {
	return Vector2(x - p.x, y - p.y);
}
inline Vector2 Vector2::operator+=(const int scalar) {
	return Vector2(x + scalar, y + scalar);
}
inline Vector2 Vector2::operator-=(const int scalar) {
	return Vector2(x - scalar, y - scalar);
}

inline Vector2 operator+(const Vector2 &p1, const float scalar) {
	return Vector2(p1.x + scalar, p1.y + scalar);
}
inline Vector2 operator-(const Vector2 &p1, const float scalar) {
	return Vector2(p1.x - scalar, p1.y - scalar);
}
inline Vector2 operator+(const float scalar, const Vector2 &p1) {
	return Vector2(p1.x + scalar, p1.y + scalar);
}
inline Vector2 operator-(const float scalar, const Vector2 &p1) {
	return Vector2(p1.x - scalar, p1.y - scalar);
}

inline Vector2 operator*(const Vector2& v1, const Vector2& v2) {
	return Vector2(v1.x * v2.x, v1.y * v2.y);
}
inline Vector2 operator*(const Vector2& v, const float scalar) {
	return Vector2(scalar * v.x, scalar * v.y);
}
inline Vector2 operator*(const float scalar, const Vector2& v) {
	return Vector2(scalar * v.x, scalar * v.y);
}
inline Vector2 operator/(const Vector2& v, const float scalar) {
	if (scalar == 0.f) {
		SystemDialogue_Okay("Divide by Zero", "Attempted to divide a Vector2 by zero", SEVERITY_FATAL);
		return Vector2(0.f, 0.f);
	}
	else
		return Vector2(v.x / scalar, v.y / scalar);
}