#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdlib.h>
#include <math.h>

#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector3.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/AABB2.hpp"

extern const float pi;

bool DoDiscsOverlap(const Vector2& center1, float r1, const Vector2& center2, float r2);
bool IsPointInDisk(const Vector2& point, const Vector2& center, float radius);

float DistanceBetweenPoints(const IntVector2& v1, const IntVector2& v2);
float DistanceBetweenPoints(const Vector2& v1, const Vector2& v2);
float DistanceBetweenPoints(const IntVector3& v1, const IntVector3& v2);
float DistanceBetweenPoints(const Vector3& v1, const Vector3& v2);

int DistanceSquaredBetweenPoints(const IntVector2& v1, const IntVector2& v2);
float DistanceSquaredBetweenPoints(const Vector2& v1, const Vector2& v2);
int DistanceSquaredBetweenPoints(const IntVector3& v1, const IntVector3& v2);
float DistanceSquaredBetweenPoints(const Vector3& v1, const Vector3& v2);

int RandInt(int min, int max);
float Randf(float Min, float Max);

float ToRadians(float degrees);
float ToDegrees(float radians);
int Quadrant(float angle);

float RangeMap(float inValue, float inStart, float inEnd, float outStart, float outEnd);
float ClampFloat(float f, float min, float max);

float CalcShortestAngularDisplacement(float fromDegrees, float toDegrees);

float CosDegrees(float radians);
float SinDegrees(float radians);

bool NumberWithinRange(float number, float start, float end);
bool NumberWithinRange(int number, int start, int end);


//FUNCTION DEFINITIONS---------------------------------------------------------------------------------------------------------------
inline float DistanceBetweenPoints(const IntVector2& v1, const IntVector2& v2) {
	return (v2 - v1).length();
}
inline float DistanceBetweenPoints(const Vector2& v1, const Vector2& v2) {
	return (v2 - v1).length();
}
inline float DistanceBetweenPoints(const IntVector3& v1, const IntVector3& v2) {
	return (v2 - v1).length();
}
inline float DistanceBetweenPoints(const Vector3& v1, const Vector3& v2) {
	return (v2 - v1).length();
}

inline bool NumberWithinRange(float number, float start, float end) {
	return number > start && number < end;
}
inline bool NumberWithinRange(int number, int start, int end) {
	return number > start && number < end;
}