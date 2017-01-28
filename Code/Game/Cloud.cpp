#include "Game/Cloud.hpp"

Cloud::Cloud()
    : m_position()
	, m_size()
	, m_speed()
{ }
Cloud::Cloud(WorldPosition pos, Vector3 size, float speed) 
    : m_position(pos)
	, m_size(size)
	, m_speed(speed)
{ }

void Cloud::UpdateCloudSpeed(float deltaSeconds) {
	m_position += Vector3(0.f, m_speed * deltaSeconds, 0.f);
}

float Cloud::GetSpeed() const {
	return m_speed;
}