#pragma once

#include "Engine/Math/Vector3.hpp"

class AABB3 {
public:
	AABB3();
	AABB3(const Vector3& mins, const Vector3& maxs);

	Vector3 mins;
	Vector3 maxs;
};