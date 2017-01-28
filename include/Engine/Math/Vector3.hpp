#pragma once

#include <math.h>
#include "Engine/ErrorWarningAssert.hpp"

class Vector3 {
public:
	//STRUCTORS
	Vector3();
	Vector3(float x, float y, float z);
	Vector3(const Vector3& other);

	float length() const;
	void normalize();

	//Vector3 operator-() const;

	//OPERATOR OVERLOADS
	inline friend Vector3 operator+(const Vector3 &p1, const Vector3 &p2);
	inline friend Vector3 operator-(const Vector3 &p1, const Vector3 &p2);

	inline Vector3 operator+=(const Vector3 &p);
	inline Vector3 operator-=(const Vector3 &p);
	inline Vector3 operator+=(const int scalar);
	inline Vector3 operator-=(const int scalar);

	inline friend Vector3 operator+(const Vector3 &p1, const float scalar);
	inline friend Vector3 operator-(const Vector3 &p1, const float scalar);
	inline friend Vector3 operator+(const float scalar, const Vector3 &p1);
	inline friend Vector3 operator-(const float scalar, const Vector3 &p1);

	inline friend Vector3 operator*(const Vector3& v1, const Vector3& v2);
	inline friend Vector3 operator*(const Vector3& v, const float scalar);
	inline friend Vector3 operator*(const float scalar, const Vector3& v);
	inline friend Vector3 operator/(const Vector3& v, const float scalar);


	//CONDITIONAL OPERATORS
	bool operator<(const Vector3 &other) const {
		if (this->x <= other.x && this->y < other.y && this->z < other.z)
			return true;
		else if (this->x < other.x && this->y <= other.y && this->z < other.z)
			return true;
		else if (this->x < other.x && this->y < other.y && this->z <= other.z)
			return true;
		else
			return false;
	}
	bool operator>(const Vector3 &other) const {
		return !(*this < other);
	}
	bool operator<=(const Vector3 &other) const {
		if (this->x <= other.x && this->y <= other.y && this->z <= other.z)
			return true;
		else
			return false;
	}
	bool operator>=(const Vector3 &other) const {
		if (this->x >= other.x && this->y >= other.y && this->z >= other.z)
			return true;
		else
			return false;
	}
	bool operator==(const Vector3 &other) const {
		if (this->x == other.x && this->y == other.y && this->z == other.z)
			return true;
		else
			return false;
	}
	bool operator!=(const Vector3 &other) const {
		return !(*this == other);
	}

public:
	static const Vector3 ZERO;
	static const Vector3 ONE;
	static const Vector3 UNIT_VECTOR_X;
	static const Vector3 UNIT_VECTOR_Y;
	static const Vector3 UNIT_VECTOR_Z;

public:
	float x;
	float y;
	float z;
};

//OPERATOR OVERLOADS
inline Vector3 operator+(const Vector3 &p1, const Vector3 &p2) {
	return Vector3(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}
inline Vector3 operator-(const Vector3 &p1, const Vector3 &p2) {
	return Vector3(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

inline Vector3 Vector3::operator+=(const Vector3 &p) {
	x += p.x;
	y += p.y;
	z += p.z;
	return *this;
}
inline Vector3 Vector3::operator-=(const Vector3 &p) {
	x -= p.x;
	y -= p.y;
	z -= p.z;
	return *this;
}
inline Vector3 Vector3::operator+=(const int scalar) {
	x += scalar;
	y += scalar;
	z += scalar;
	return *this;
}
inline Vector3 Vector3::operator-=(const int scalar) {
	x -= scalar;
	y -= scalar;
	z -= scalar;
	return *this;
}

inline Vector3 operator+(const Vector3 &p1, const float scalar) {
	return Vector3(p1.x + scalar, p1.y + scalar, p1.z + scalar);
}
inline Vector3 operator-(const Vector3 &p1, const float scalar) {
	return Vector3(p1.x - scalar, p1.y - scalar, p1.z - scalar);
}
inline Vector3 operator+(const float scalar, const Vector3 &p1) {
	return Vector3(p1.x + scalar, p1.y + scalar, p1.z + scalar);
}
inline Vector3 operator-(const float scalar, const Vector3 &p1) {
	return Vector3(p1.x - scalar, p1.y - scalar, p1.z - scalar);
}


inline Vector3 operator*(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}
inline Vector3 operator*(const Vector3& v, const float scalar) {
	return Vector3(scalar * v.x, scalar * v.y, scalar * v.z);
}
inline Vector3 operator*(const float scalar, const Vector3& v) {
	return Vector3(scalar * v.x, scalar * v.y, scalar * v.z);
}
inline Vector3 operator/(const Vector3& v, const float scalar) {
	if (scalar == 0.f) {
		SystemDialogue_Okay("Divide by Zero", "Attempted to divide a Vector3 by zero", SEVERITY_FATAL);
		return Vector3(0.f, 0.f, 0.f);
	}
	else
		return Vector3(v.x / scalar, v.y / scalar, v.z / scalar);
}