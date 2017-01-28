#pragma once

#include "Engine/Math/Vector3.hpp"
#include "Game/GameCommon.hpp"

class Cloud {
public:
	Cloud();
	Cloud(WorldPosition pos, Vector3 size, float speed);

	void UpdateCloudSpeed(float deltaSeconds);

	float GetSpeed() const;

public:
	Vector3 m_position;
	Vector3 m_size;

private:
	float m_speed;
};