#include "MathUtils.hpp"

const float pi = 3.1415926535897932384626433832795f;

bool DoDiscsOverlap(const Vector2& center1, float r1, const Vector2& center2, float r2) {
	float dist = DistanceBetweenPoints(center1, center2);
	float radii = r1 + r2;

	return (dist < radii);
}
bool IsPointInDisk(const Vector2& point, const Vector2& center, float radius) {
	if (DistanceBetweenPoints(point, center) < radius)
		return true;
	else
		return false;
}

int DistanceSquaredBetweenPoints(const IntVector2& v1, const IntVector2& v2) {
	int xDist = v2.x - v1.x;
	int yDist = v2.y - v1.y;
	return xDist * xDist + yDist * yDist;
}
float DistanceSquaredBetweenPoints(const Vector2& v1, const Vector2& v2) {
	float xDist = v2.x - v1.x;
	float yDist = v2.y - v1.y;
	return xDist * xDist + yDist * yDist;
}
int DistanceSquaredBetweenPoints(const IntVector3& v1, const IntVector3& v2) {
	int xDist = v2.x - v1.x;
	int yDist = v2.y - v1.y;
	return xDist * xDist + yDist * yDist;
}
float DistanceSquaredBetweenPoints(const Vector3& v1, const Vector3& v2) {
	float xDist = v2.x - v1.x;
	float yDist = v2.y - v1.y;
	return xDist * xDist + yDist * yDist;
}

int RandInt(int min, int max) {
	return ((rand()) / RAND_MAX * (max - min)) + min;
}
float Randf(float Min, float Max) {
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

float ToRadians(float degrees) {
	return degrees * (pi / 180.f);
}
float ToDegrees(float radians) {
	return radians * (180.f / pi);
}
int Quadrant(float angle) {
	if (cos(angle) > 0 && sin(angle) > 0)
		return 1;
	else if (cos(angle) < 0 && sin(angle) > 0)
		return 2;
	else if (cos(angle) < 0 && sin(angle) < 0)
		return 3;
	else if (cos(angle) > 0 && sin(angle) < 0)
		return 4;
	else
		return -1;
}

float RangeMap(float inValue, float inStart, float inEnd, float outStart, float outEnd) {
	return (((inValue - inStart) / (inEnd - inStart)) * (outEnd - outStart)) + outStart;
}
float ClampFloat(float f, float min, float max) {
	if (f < min)
		return min;
	else if (f > max)
		return max;
	else
		return f;
} 

float CalcShortestAngularDisplacement(float fromDegrees, float toDegrees) {
	float angularDisplacement = toDegrees - fromDegrees;

	while (angularDisplacement > 180.f)
		angularDisplacement -= 360.f;
	while (angularDisplacement < -180.f)
		angularDisplacement += 360.f;

	return angularDisplacement;
}

float CosDegrees(float degrees) {
	return cos(degrees * (pi / 180.f));
}
float SinDegrees(float degrees) {
	return sin(degrees * (pi / 180.f));
}