#pragma once
#include <math.h>
#include "Engine/ErrorWarningAssert.hpp"

class IntVector2 {
public:
	IntVector2();
	IntVector2(int x, int y);
	IntVector2(const IntVector2& other);

	float length() const;

	IntVector2 operator-() const;

	//OPERATOR OVERLOADS
	inline friend IntVector2 operator+(const IntVector2 &p1, const IntVector2 &p2);
	inline friend IntVector2 operator-(const IntVector2 &p1, const IntVector2 &p2);

	inline friend IntVector2 operator+(const IntVector2 &p1, const int scalar);
	inline friend IntVector2 operator-(const IntVector2 &p1, const int scalar);
	inline friend IntVector2 operator+(const int scalar, const IntVector2 &p1);
	inline friend IntVector2 operator-(const int scalar, const IntVector2 &p1);

	inline IntVector2 operator+=(const IntVector2 &p);
	inline IntVector2 operator-=(const IntVector2 &p);
	inline IntVector2 operator+=(const int scalar);
	inline IntVector2 operator-=(const int scalar);


	inline friend IntVector2 operator*(const IntVector2& v1, const IntVector2& v2);
	inline friend IntVector2 operator*(const IntVector2& v, const int scalar);
	inline friend IntVector2 operator*(const int scalar, const IntVector2& v);
	inline friend IntVector2 operator/(const IntVector2& v, const int scalar);

	//CONDITIONAL OPERATORS
	//THESE ARE WRONG
	bool operator<(const IntVector2 &other) const {
		if (this->x < other.x)
			return true;
		else if (this->x > other.x)
			return false;
		else if (this->x == other.x) {
			if (this->y < other.y)
				return true;
			else if (this->y >= other.y)
				return false;
			else
				return false;
		}
		else
			return false;
	}
	bool operator>(const IntVector2 &other) const {
		return !(*this < other);
	}
	bool operator<=(const IntVector2 &other) const {
		if (this->x <= other.x && this->y <= other.y)
			return true;
		else
			return false;
	}
	bool operator>=(const IntVector2 &other) const {
		if (this->x >= other.x && this->y >= other.y)
			return true;
		else
			return false;
	}
	bool operator==(const IntVector2 &other) const {
		if (this->x == other.x && this->y == other.y)
			return true;
		else
			return false;
	}
	bool operator!=(const IntVector2 &other) const {
		return !(*this == other);
	}

public:
	int x;
	int y;
};

inline IntVector2 operator+(const IntVector2 &p1, const IntVector2 &p2) {
	return IntVector2(p1.x + p2.x, p1.y + p2.y);
}
inline IntVector2 operator-(const IntVector2 &p1, const IntVector2 &p2) {
	return IntVector2(p1.x - p2.x, p1.y - p2.y);
}

inline IntVector2 IntVector2::operator+=(const IntVector2 &p) {
	return IntVector2(x + p.x, y + p.y);
}
inline IntVector2 IntVector2::operator-=(const IntVector2 &p) {
	return IntVector2(x - p.x, y - p.y);
}
inline IntVector2 IntVector2::operator+=(const int scalar) {
	return IntVector2(x + scalar, y + scalar);
}
inline IntVector2 IntVector2::operator-=(const int scalar) {
	return IntVector2(x - scalar, y - scalar);
}

inline IntVector2 operator+(const IntVector2 &p1, const int scalar) {
	return IntVector2(p1.x + scalar, p1.y + scalar);
}
inline IntVector2 operator-(const IntVector2 &p1, const int scalar) {
	return IntVector2(p1.x - scalar, p1.y - scalar);
}
inline IntVector2 operator+(const int scalar, const IntVector2 &p1) {
	return IntVector2(p1.x + scalar, p1.y + scalar);
}
inline IntVector2 operator-(const int scalar, const IntVector2 &p1) {
	return IntVector2(p1.x - scalar, p1.y - scalar);
}

inline IntVector2 operator*(const IntVector2& v1, const IntVector2& v2) {
	return IntVector2(v1.x * v2.x, v1.y * v2.y);
}
inline IntVector2 operator*(const IntVector2& v, const int scalar) {
	return IntVector2(v.x * scalar, v.y * scalar);
}
inline IntVector2 operator*(const int scalar, const IntVector2& v) {
	return IntVector2(v.x * scalar, v.y * scalar);
}
inline IntVector2 operator/(const IntVector2& v, const int scalar) {
	if (scalar == 0.f) {
		SystemDialogue_Okay("Divide by Zero", "Attempted to divide a IntVector2 by zero", SEVERITY_FATAL);
		return IntVector2(0, 0);
	}
	else
		return IntVector2(v.x / scalar, v.y / scalar);
}