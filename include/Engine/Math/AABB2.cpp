#include "Engine/Math/AABB2.hpp"

AABB2::AABB2() {

}
AABB2::AABB2(const Vector2& mins, const Vector2& maxs) {
	this->mins = mins;
	this->maxs = maxs;
}