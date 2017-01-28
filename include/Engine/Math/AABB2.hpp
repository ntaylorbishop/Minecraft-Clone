#pragma once

#include "Engine/Math/Vector2.hpp"

class AABB2 {
public:
	AABB2();
	AABB2(const Vector2& mins, const Vector2& maxs);


	Vector2 mins;
	Vector2 maxs;
};