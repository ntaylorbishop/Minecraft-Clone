#pragma once

#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/VectorUtils.hpp"
#include "Game/TheWorld.hpp"
#include "Game/Chunk.hpp"


struct RaycastResult {
	Vector3 m_impactPos;
	Vector3 m_prevImpactBlockPos;
	Vector3 m_prevImpactPos;
	Block* m_impactBlock = nullptr;
	BlockFace m_impactSurfaceNormal;
	bool m_didImpact = false;
};

bool Raycast3DToPoint(const Vector3& start, const Vector3& end, RaycastResult* out_result); //Returns a bool and a raycast result if it hits something. If it doesn't, returns a null raycast result
