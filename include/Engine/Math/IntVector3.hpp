#pragma once
#include <math.h>
#include "Engine/ErrorWarningAssert.hpp"

class IntVector3 {
public:
	IntVector3();
	IntVector3(int x, int y, int z);
	IntVector3(const IntVector3& other);

	float length() const;

	inline IntVector3 operator-() const;

	//OPERATOR OVERLOADS
	inline friend IntVector3 operator+(const IntVector3 &p1, const IntVector3 &p2);
	inline friend IntVector3 operator-(const IntVector3 &p1, const IntVector3 &p2);

	inline IntVector3 operator+=(const IntVector3 &p);
	inline IntVector3 operator-=(const IntVector3 &p);
	inline IntVector3 operator+=(const int scalar);
	inline IntVector3 operator-=(const int scalar);

	inline friend IntVector3 operator+(const IntVector3 &p1, const int scalar);
	inline friend IntVector3 operator-(const IntVector3 &p1, const int scalar);
	inline friend IntVector3 operator+(const int scalar, const IntVector3 &p1);
	inline friend IntVector3 operator-(const int scalar, const IntVector3 &p1);

	inline friend IntVector3 operator*(const IntVector3& v1, const IntVector3& v2);
	inline friend IntVector3 operator*(const IntVector3& v, const int scalar);
	inline friend IntVector3 operator*(const int scalar, const IntVector3& v);
	inline friend IntVector3 operator/(const IntVector3& v, const int scalar);

	//CONDITIONAL OPERATORS
	bool operator<(const IntVector3 &other) const {
		if (x <= other.x && y < other.y && z < other.z)
			return true;
		else if (x < other.x && y <= other.y && z < other.z)
			return true;
		else if (x < other.x && y < other.y && z <= other.z)
			return true;
		else
			return false;
	}
	bool operator>(const IntVector3 &other) const {
		return !(*this < other);
	}
	bool operator<=(const IntVector3 &other) const {
		if (x <= other.x && y <= other.y && z <= other.z)
			return true;
		else
			return false;
	}
	bool operator>=(const IntVector3 &other) const {
		if (x >= other.x && y >= other.y && z >= other.z)
			return true;
		else
			return false;
	}
	bool operator==(const IntVector3 &other) const {
		if (x == other.x && y == other.y && z == other.z)
			return true;
		else
			return false;
	}
	bool operator!=(const IntVector3 &other) const {
		return !(*this == other);
	}

public:
	int x;
	int y;
	int z;
};

inline IntVector3 operator+(const IntVector3 &p1, const IntVector3 &p2) {
	return IntVector3(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}
inline IntVector3 operator-(const IntVector3 &p1, const IntVector3 &p2) {
	return IntVector3(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

inline IntVector3 IntVector3::operator+=(const IntVector3 &p) {
	return IntVector3(x + p.x, y + p.y, z + p.z);
}
inline IntVector3 IntVector3::operator-=(const IntVector3 &p) {
	return IntVector3(x - p.x, y - p.y, z - p.z);
}
inline IntVector3 IntVector3::operator+=(const int scalar) {
	return IntVector3(x + scalar, y + scalar, z + scalar);
}
inline IntVector3 IntVector3::operator-=(const int scalar) {
	return IntVector3(x - scalar, y - scalar, z - scalar);
}

inline IntVector3 operator+(const IntVector3 &p1, const int scalar) {
	return IntVector3(p1.x + scalar, p1.y + scalar, p1.z + scalar);
}
inline IntVector3 operator-(const IntVector3 &p1, const int scalar) {
	return IntVector3(p1.x - scalar, p1.y - scalar, p1.z - scalar);
}
inline IntVector3 operator+(const int scalar, const IntVector3 &p1) {
	return IntVector3(p1.x + scalar, p1.y + scalar, p1.z + scalar);
}
inline IntVector3 operator-(const int scalar, const IntVector3 &p1) {
	return IntVector3(p1.x - scalar, p1.y - scalar, p1.z - scalar);
}

inline IntVector3 operator*(const IntVector3& v1, const IntVector3& v2) {
	return IntVector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}
inline IntVector3 operator*(const IntVector3& v, const int scalar) {
	return IntVector3(v.x * scalar, v.y * scalar, v.z * scalar);
}
inline IntVector3 operator*(const int scalar, const IntVector3& v) {
	return IntVector3(v.x * scalar, v.y * scalar, v.z * scalar);
}
inline IntVector3 operator/(const IntVector3& v, const float scalar) {
	if (scalar == 0.f) {
		SystemDialogue_Okay("Divide by Zero", "Attempted to divide a IntVector3 by zero", SEVERITY_FATAL);
		return IntVector3(0, 0, 0);
	}
	else
		return IntVector3((int)(v.x / scalar), (int)(v.y / scalar), (int)(v.z / scalar));
}