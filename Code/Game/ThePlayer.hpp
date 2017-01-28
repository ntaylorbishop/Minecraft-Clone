#pragma once

#include <string>
#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Game/TheWorld.hpp"
#include "Game/Raycast3D.hpp"
#include "Game/AudioPlayer.hpp"

class ThePlayer;
extern ThePlayer* g_thePlayer;

const float PLAYER_HEIGHT = 1.85f;
const float PLAYER_RADIUS = 0.3f;

enum MovementMode {
	MOVEMENT_MODE_NOCLIP = 0,
	MOVEMENT_MODE_FLYING,
	MOVEMENT_MODE_WALKING
};

using namespace std;
class ThePlayer {
public:
	//STRUCTORS
	ThePlayer();
	ThePlayer(const WorldPosition& position, const Vector3& eulerOrientation, MovementMode mode = MOVEMENT_MODE_FLYING);
	~ThePlayer();

	//UPDATE
	void Update(float deltaSeconds);
	void UpdatePlayerPosition(float deltaSeconds);

	void Jump();

	//RENDER
	void Render() const;

	void UpdateVertexArrays();

	//ACCESSORS
	Vector3 GetForwardXYZ() const;
	Vector3 GetForwardXY() const;
	Vector3 GetLeftXY() const;

	Vector3 GetForwardDisplacedByValue(float dist, const Vector3& dispFromCenter = Vector3(0.f, 0.f, 0.f)) const;

	MovementMode GetMovementMode() const;
	string GetMovementModeStringified() const;
	void SetMovementMode(MovementMode mode);

	void IsRunning(bool isShiftBeingPressed);

	bool IsGrounded() const;
	vector<BlockType> GetBlockTypesOfBlocksBelowPlayer();

public:
	Vector3 m_acceleration;
	Vector3 m_position;
	Vector3 m_velocity;

	float m_rollAboutX;
	float m_pitchAboutYDeg;
	float m_yawAboutZ;

private:
	float m_friction;
	float m_maxVelocity;
	float m_currMaxVelocity;
	float m_accelerationDueToJumping;

	bool m_isGrounded;

	vector<Vertex> m_verticesTopCircle;
	vector<Vertex> m_verticesBottomCircle;
	vector<Vertex> m_verticesConnectingLines;
	vector<Vertex> m_raycastVertices;

	MovementMode m_movementMode;

	//PRIVATE UTILITY
	void FixAndClampAngles();

//COLLISION DETECTION
	void CheckBlockCollisions();

	void CheckAndPushFromAdjacentBlocks();

	void CheckAndPushFromDiagonalAboveBlocks();
	void CheckAndPushFromDiagonalBelowBlocks();

	void CheckAndPushFromMiddleCornerBlocks();
	void CheckAndPushFromAboveCornerBlocks();
	void CheckAndPushFromBelowCornerBlocks();

	//COLLISION UTILITY
	void PushPlayerFromPoint(const Vector2& pointOnBlock);
};