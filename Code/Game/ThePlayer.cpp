#include "Game/TheCamera.hpp"
#include "Game/ThePlayer.hpp"

ThePlayer* g_thePlayer = nullptr;

//STRUCTORS-------------------------------------------------------------------------------------------------------------------------------------
ThePlayer::ThePlayer() 
    : m_position(Vector3(0.f, 0.f, 0.f))
	, m_rollAboutX(0.f)
	, m_pitchAboutYDeg(0.f)
	, m_yawAboutZ(0.f)
	, m_movementMode(MOVEMENT_MODE_FLYING)
	, m_velocity(Vector3(0.f, 0.f, 0.f))
	, m_acceleration(Vector3(0.f, 0.f, 0.f))
	, m_friction(0.f)
	, m_maxVelocity(0.f)
	, m_accelerationDueToJumping(0.f)
	, m_isGrounded(false)
	, m_currMaxVelocity(0.f)
{
	UpdateVertexArrays();
}
ThePlayer::ThePlayer(const WorldPosition& position, const Vector3& eulerOrientation, MovementMode mode)
    : m_position(position)
    , m_rollAboutX(eulerOrientation.x)
    , m_pitchAboutYDeg(eulerOrientation.y)
    , m_yawAboutZ(eulerOrientation.z)
    , m_movementMode(mode)
    , m_velocity(Vector3(0.f, 0.f, 0.f))
    , m_acceleration(Vector3(0.f, 0.f, 0.f))
	, m_friction(0.f)
	, m_maxVelocity(0.f)
	, m_accelerationDueToJumping(0.f)
	, m_isGrounded(false)
	, m_currMaxVelocity(0.f)
{
	UpdateVertexArrays();
}
ThePlayer::~ThePlayer() {

}

//UPDATE-------------------------------------------------------------------------------------------------------------------------------------
void ThePlayer::Update(float deltaSeconds) {

	UpdatePlayerPosition(deltaSeconds);
	UpdateVertexArrays();
	FixAndClampAngles();
}

void ThePlayer::UpdatePlayerPosition(float deltaSeconds) {

	//Update acceleration
	if (m_movementMode == MOVEMENT_MODE_WALKING) {
		m_acceleration.z += -PLAYER_GRAVITY;

		if (m_acceleration.z < -PLAYER_GRAVITY)
			m_acceleration.z = -PLAYER_GRAVITY;
	}

	m_velocity += m_acceleration;

	//Update velocity
	if (m_movementMode == MOVEMENT_MODE_WALKING)
 		m_maxVelocity = m_currMaxVelocity;
 	else
 		m_maxVelocity = PLAYER_MAX_VELOCITY_NOCLIP;

	//Clamp velocity
	if (m_movementMode != MOVEMENT_MODE_WALKING) {
		if (m_velocity.length() >= m_maxVelocity) {
			m_velocity.normalize();

			m_velocity = m_velocity * m_maxVelocity;
		}
	}
	else {
		Vector2 velocity2D = Vector2(m_velocity.x, m_velocity.y);
		if (velocity2D.length() >= m_maxVelocity) {
			velocity2D.normalize();

			velocity2D = velocity2D * m_maxVelocity;
			m_velocity.x = velocity2D.x;
			m_velocity.y = velocity2D.y;
		}
	}

	m_velocity = m_velocity * PLAYER_FRICTION_COEFFICIENT;

	//Update position and play footstep sound
	Vector3 vecToMoveBy = m_velocity;

	Vector3 lastPos = m_position;
	m_position += vecToMoveBy * deltaSeconds;

	if (m_movementMode != MOVEMENT_MODE_NOCLIP)
		CheckBlockCollisions();

	float distToMove = (m_position - lastPos).length();
	if (IsGrounded() && m_movementMode != MOVEMENT_MODE_NOCLIP && m_movementMode != MOVEMENT_MODE_FLYING)
		g_theAudioPlayer->PlayFootsteps(distToMove, GetBlockTypesOfBlocksBelowPlayer());
}

void ThePlayer::Jump() {
	if (m_isGrounded) {
		m_acceleration.z += PLAYER_JUMP_SPEED;
		m_isGrounded = false;
	}
}

//RENDER-------------------------------------------------------------------------------------------------------------------------------------
void ThePlayer::Render() const {
	g_theRenderer->SetColor(RGBA::WHITE);
	g_theRenderer->DrawVertexArray(m_verticesTopCircle, m_verticesTopCircle.size(), PRIMITIVE_LINE_LOOP);
	g_theRenderer->DrawVertexArray(m_verticesBottomCircle, m_verticesBottomCircle.size(), PRIMITIVE_LINE_LOOP);
	g_theRenderer->DrawVertexArray(m_verticesConnectingLines, m_verticesConnectingLines.size(), PRIMITIVE_LINES);

	g_theRenderer->SetColor(RGBA::MAGENTA);

	if (g_theCamera->GetCameraMode() != CAMERA_MODE_FIRST_PERSON)
		g_theRenderer->DrawVertexArray(m_raycastVertices, m_verticesBottomCircle.size(), PRIMITIVE_LINES);
}

void ThePlayer::UpdateVertexArrays() {
	float circleAccuracy = 50;
	float angleStep = 360.f / (float)circleAccuracy;

	m_verticesTopCircle.clear();
	m_verticesBottomCircle.clear();
	m_verticesConnectingLines.clear();
	m_raycastVertices.clear();

	//Push top circle vertices
	for (int i = 0; i < circleAccuracy; i++) {
		float angle = angleStep * (float)i;

		Vector3 topVertPos    = Vector3(PLAYER_RADIUS * CosDegrees(angle), PLAYER_RADIUS * SinDegrees(angle), 0.f) + m_position + Vector3(0.f, 0.f, PLAYER_HEIGHT / 2.f);
		Vector3 bottomVertPos = Vector3(PLAYER_RADIUS * CosDegrees(angle), PLAYER_RADIUS * SinDegrees(angle), 0.f) + m_position - Vector3(0.f, 0.f, PLAYER_HEIGHT / 2.f);

		m_verticesTopCircle.push_back(Vertex(topVertPos, RGBA::WHITE));
		m_verticesBottomCircle.push_back(Vertex(bottomVertPos, RGBA::WHITE));

		if (g_theCamera->GetCameraMode() == CAMERA_MODE_THIRD_PERSON || g_theCamera->GetCameraMode() == CAMERA_MODE_SPECTATOR) {
			m_verticesConnectingLines.push_back(Vertex(topVertPos, RGBA::WHITE));
			m_verticesConnectingLines.push_back(Vertex(bottomVertPos, RGBA::WHITE));
		}
	}

	m_verticesConnectingLines.push_back(Vertex(m_position + Vector3(0.f, 0.f, PLAYER_HEIGHT / 2.f), RGBA::WHITE));
	m_verticesConnectingLines.push_back(Vertex(m_position - Vector3(0.f, 0.f, PLAYER_HEIGHT / 2.f), RGBA::WHITE));

	//Push raycasts
	Vector3 raycastStart = m_position + Vector3(0.f, 0.f, 0.69f);;
	Vector3 raycastEnd = GetForwardDisplacedByValue(PLAYER_SELECTION_RANGE, Vector3(0.f, 0.f, 0.69f));

	m_raycastVertices.push_back(Vertex(raycastStart, RGBA::MAGENTA));
	m_raycastVertices.push_back(Vertex(raycastEnd, RGBA::MAGENTA));
}

//ACCESSORS-------------------------------------------------------------------------------------------------------------------------------------
Vector3 ThePlayer::GetForwardXYZ() const {
	return Vector3(CosDegrees(m_yawAboutZ) * CosDegrees(m_pitchAboutYDeg), SinDegrees(m_yawAboutZ) * CosDegrees(m_pitchAboutYDeg), SinDegrees(-m_pitchAboutYDeg));
}
Vector3 ThePlayer::GetForwardXY() const {
	return Vector3(CosDegrees(m_yawAboutZ), SinDegrees(m_yawAboutZ), 0.f);
}
Vector3 ThePlayer::GetLeftXY() const {
	Vector3 forwardXY = GetForwardXY();
	return Vector3(-forwardXY.y, forwardXY.x, 0.f);
}

Vector3 ThePlayer::GetForwardDisplacedByValue(float dist, const Vector3& dispFromCenter) const {
	Vector3 start = m_position + dispFromCenter;
	Vector3 end = GetForwardXYZ();

	end.normalize();
	end = IncreaseVectorByLength(end, dist);
	end += start;

	return end;
}

MovementMode ThePlayer::GetMovementMode() const {
	return m_movementMode;
}
string ThePlayer::GetMovementModeStringified() const {
	if (m_movementMode == MOVEMENT_MODE_FLYING)
		return "Flying";
	else if (m_movementMode == MOVEMENT_MODE_NOCLIP)
		return "No Clip";
	else if (m_movementMode == MOVEMENT_MODE_WALKING)
		return "Walking";
	else
		return "ERROR";
}
void ThePlayer::SetMovementMode(MovementMode mode) {
	m_movementMode = mode;
}

bool ThePlayer::IsGrounded() const {
	return m_isGrounded;
}

void ThePlayer::IsRunning(bool isShiftBeingPressed) {
	if (isShiftBeingPressed) {
		m_currMaxVelocity = PLAYER_MAX_VELOCITY_WALKING + PLAYER_SHIFT_MOVE_SPEED;
		g_isRunning = true;
	}
	else {
		m_currMaxVelocity = PLAYER_MAX_VELOCITY_WALKING;
		g_isRunning = false;
	}
}

vector<BlockType> ThePlayer::GetBlockTypesOfBlocksBelowPlayer() {
	vector<BlockType> blockTypesOfBlocksBelow;

	Vector3 pointToCheck_TopLeft = m_position + Vector3(PLAYER_RADIUS, PLAYER_RADIUS, 0.f);
	Vector3 pointToCheck_TopRight = m_position + Vector3(-PLAYER_RADIUS, PLAYER_RADIUS, 0.f);
	Vector3 pointToCheck_BottomRight = m_position + Vector3(PLAYER_RADIUS, -PLAYER_RADIUS, 0.f);
	Vector3 pointToCheck_BottomLeft = m_position + Vector3(-PLAYER_RADIUS, -PLAYER_RADIUS, 0.f);

	BlockInfo block_TopLeft = g_theWorld->GetBlockInfoForWorldPosition(pointToCheck_BottomLeft).GetBlockBelow();
	BlockInfo block_TopRight = g_theWorld->GetBlockInfoForWorldPosition(pointToCheck_TopRight).GetBlockBelow();
	BlockInfo block_BottomRight = g_theWorld->GetBlockInfoForWorldPosition(pointToCheck_BottomRight).GetBlockBelow();
	BlockInfo block_BottomLeft = g_theWorld->GetBlockInfoForWorldPosition(pointToCheck_BottomLeft).GetBlockBelow();

	if (block_TopLeft.GetBlock())
		blockTypesOfBlocksBelow.push_back(block_TopLeft.GetBlock()->GetBlockType());

	if (block_TopRight.GetBlock())
		blockTypesOfBlocksBelow.push_back(block_BottomLeft.GetBlock()->GetBlockType());

	if (block_BottomRight.GetBlock())
		blockTypesOfBlocksBelow.push_back(block_BottomRight.GetBlock()->GetBlockType());

	if (block_BottomLeft.GetBlock())
		blockTypesOfBlocksBelow.push_back(block_BottomLeft.GetBlock()->GetBlockType());

	return blockTypesOfBlocksBelow;
}

//PRIVATE UTILITY-------------------------------------------------------------------------------------------------------------------------------------
void ThePlayer::FixAndClampAngles() {
	if (m_pitchAboutYDeg > 89.9f)
		m_pitchAboutYDeg = 89.9f;
	else if (m_pitchAboutYDeg < -89.9f)
		m_pitchAboutYDeg = -89.9f;

	if (m_rollAboutX > 89.9f)
		m_rollAboutX = 89.9f;
	else if (m_rollAboutX < -89.9f)
		m_rollAboutX = -89.9f;

	while (m_yawAboutZ < -180.f)
		m_yawAboutZ += 360.f;
	while (m_yawAboutZ > 180.f)
		m_yawAboutZ -= 360.f;
}

//COLLISION DETECTION-------------------------------------------------------------------------------------------------------------------------------------
void ThePlayer::CheckBlockCollisions() {

	m_isGrounded = false;

	if (!(m_movementMode == MOVEMENT_MODE_FLYING && m_position.z > 128.f)) {
		CheckAndPushFromAdjacentBlocks();

		CheckAndPushFromDiagonalAboveBlocks();
		CheckAndPushFromDiagonalBelowBlocks();

		CheckAndPushFromMiddleCornerBlocks();
		CheckAndPushFromAboveCornerBlocks();
		CheckAndPushFromBelowCornerBlocks();
	}
}

void ThePlayer::CheckAndPushFromAdjacentBlocks() {
	BlockInfo belowBlock = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockBelow();
	BlockInfo aboveBlock = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockAbove();
	BlockInfo northBlock = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockToNorth();
	BlockInfo southBlock = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockToSouth();
	BlockInfo westBlock = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockToWest();
	BlockInfo eastBlock = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockToEast();

	if (belowBlock.GetBlock()) {
		float playerPointToCheck_Below = m_position.z - PLAYER_HEIGHT / 2.f;
		float blockPointToCheck_Below = belowBlock.GetBlockWorldPosition().z + 1.f;

		if (BlockDefinition::IsSolid(belowBlock.GetBlock()->GetBlockType()) && playerPointToCheck_Below < blockPointToCheck_Below) {
			m_position.z += abs(blockPointToCheck_Below - playerPointToCheck_Below);
			m_isGrounded = true;
		}
	}

	if (aboveBlock.GetBlock() && m_position.z < BLOCKS_WIDE_Z) {
		float playerPointToCheck_Above = m_position.z + PLAYER_HEIGHT / 2.f;
		float blockPointToCheck_Above = aboveBlock.GetBlockWorldPosition().z;

		if (BlockDefinition::IsSolid(aboveBlock.GetBlock()->GetBlockType()) && playerPointToCheck_Above > blockPointToCheck_Above)
			m_position.z -= abs(blockPointToCheck_Above - playerPointToCheck_Above);
	}

	if (northBlock.GetBlock()) {
		float playerPointToCheck_North = m_position.x - PLAYER_RADIUS;
		float blockPointToCheck_North = northBlock.GetBlockWorldPosition().x + 1.f;


		if (BlockDefinition::IsSolid(northBlock.GetBlock()->GetBlockType()) && playerPointToCheck_North < blockPointToCheck_North) {
			m_position.x += abs(blockPointToCheck_North - playerPointToCheck_North);
			m_velocity.x = 0.f;
		}
	}

	if (southBlock.GetBlock()) {
		float playerPointToCheck_South = m_position.x + PLAYER_RADIUS;
		float blockPointToCheck_South = southBlock.GetBlockWorldPosition().x;

		if (BlockDefinition::IsSolid(southBlock.GetBlock()->GetBlockType()) && playerPointToCheck_South > blockPointToCheck_South) {
			m_position.x -= abs(blockPointToCheck_South - playerPointToCheck_South);
			m_velocity.x = 0.f;
		}
	}

	if (westBlock.GetBlock()) {
		float playerPointToCheck_West = m_position.y - PLAYER_RADIUS;
		float blockPointToCheck_West = westBlock.GetBlockWorldPosition().y + 1.f;

		if (BlockDefinition::IsSolid(westBlock.GetBlock()->GetBlockType()) && playerPointToCheck_West < blockPointToCheck_West) {
			m_position.y += abs(blockPointToCheck_West - playerPointToCheck_West);
			m_velocity.y = 0.f;
		}
	}


	if (eastBlock.GetBlock()) {
		float playerPointToCheck_East = m_position.y + PLAYER_RADIUS;
		float blockPointToCheck_East = eastBlock.GetBlockWorldPosition().y;

		if (BlockDefinition::IsSolid(eastBlock.GetBlock()->GetBlockType()) && playerPointToCheck_East > blockPointToCheck_East) {
			m_position.y -= abs(blockPointToCheck_East - playerPointToCheck_East);
			m_velocity.y = 0.f;
		}
	}
}

void ThePlayer::CheckAndPushFromDiagonalAboveBlocks() {
	BlockInfo block_NorthAbove = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockAbove().GetBlockToNorth();
	BlockInfo block_SouthAbove = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockAbove().GetBlockToSouth();
	BlockInfo block_WestAbove =  g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockAbove().GetBlockToWest();
	BlockInfo block_EastAbove =  g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockAbove().GetBlockToEast();

	if (block_NorthAbove.GetBlock()) {
		float playerPointToCheck_NorthAbove = m_position.x - PLAYER_RADIUS;
		float blockPointToCheck_NorthAbove = block_NorthAbove.GetBlockWorldPosition().x + 1.f;

		if (BlockDefinition::IsSolid(block_NorthAbove.GetBlock()->GetBlockType()) && playerPointToCheck_NorthAbove < blockPointToCheck_NorthAbove) {
			m_position.x += abs(playerPointToCheck_NorthAbove - blockPointToCheck_NorthAbove);
			m_velocity.x = 0.f;
		}
	}

	if (block_SouthAbove.GetBlock()) {
		float playerPointToCheck_SouthAbove = m_position.x + PLAYER_RADIUS;
		float blockPointToCheck_SouthAbove = block_SouthAbove.GetBlockWorldPosition().x;

		if (BlockDefinition::IsSolid(block_SouthAbove.GetBlock()->GetBlockType()) && playerPointToCheck_SouthAbove > blockPointToCheck_SouthAbove) {
			m_position.x -= abs(blockPointToCheck_SouthAbove - playerPointToCheck_SouthAbove);
			m_velocity.x = 0.f;
		}
	}

	if (block_WestAbove.GetBlock()) {
		float playerPointToCheck_WestAbove = m_position.y - PLAYER_RADIUS;
		float blockPointToCheck_WestAbove = block_WestAbove.GetBlockWorldPosition().y + 1.f;

		if (BlockDefinition::IsSolid(block_WestAbove.GetBlock()->GetBlockType()) && playerPointToCheck_WestAbove < blockPointToCheck_WestAbove) {
			m_position.y += abs(blockPointToCheck_WestAbove - playerPointToCheck_WestAbove);
			m_velocity.y = 0.f;
		}
	}

	if (block_EastAbove.GetBlock()) {
		float playerPointToCheck_EastAbove = m_position.y + PLAYER_RADIUS;
		float blockPointToCheck_EastAbove = block_EastAbove.GetBlockWorldPosition().y;

		if (BlockDefinition::IsSolid(block_EastAbove.GetBlock()->GetBlockType()) && playerPointToCheck_EastAbove > blockPointToCheck_EastAbove) {
			m_position.y -= abs(blockPointToCheck_EastAbove - playerPointToCheck_EastAbove);
			m_velocity.y = 0.f;
		}
	}
	

}
void ThePlayer::CheckAndPushFromDiagonalBelowBlocks() {
	BlockInfo block_NorthBelow = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockBelow().GetBlockToNorth();
	BlockInfo block_SouthBelow = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockBelow().GetBlockToSouth();
	BlockInfo block_WestBelow =  g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockBelow().GetBlockToWest();
	BlockInfo block_EastBelow =  g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockBelow().GetBlockToEast();

	if (block_NorthBelow.GetBlock()) {
		Vector3 playerPointToCheck_NorthBelow = m_position + Vector3(-PLAYER_RADIUS, 0.f, -(PLAYER_HEIGHT / 2.f));
		Vector3 blockPointToCheck_NorthBelow = block_NorthBelow.GetBlockWorldPosition() + Vector3(1.f, 0.f, 1.f);

		if (BlockDefinition::IsSolid(block_NorthBelow.GetBlock()->GetBlockType())) {

			if (playerPointToCheck_NorthBelow.x < blockPointToCheck_NorthBelow.x && playerPointToCheck_NorthBelow.z < blockPointToCheck_NorthBelow.z) {
				m_position.z += abs(playerPointToCheck_NorthBelow.z - blockPointToCheck_NorthBelow.z);
				m_isGrounded = true;
			}
		}
	}

	if (block_SouthBelow.GetBlock()) {
		Vector3 playerPointToCheck_SouthBelow = m_position + Vector3(PLAYER_RADIUS, 0.f, -(PLAYER_HEIGHT / 2.f));
		Vector3 blockPointToCheck_SouthBelow = block_SouthBelow.GetBlockWorldPosition() + Vector3(0.f, 0.f, 1.f);

		if (BlockDefinition::IsSolid(block_SouthBelow.GetBlock()->GetBlockType())) {

			if (playerPointToCheck_SouthBelow.x > blockPointToCheck_SouthBelow.x && playerPointToCheck_SouthBelow.z < blockPointToCheck_SouthBelow.z) {
				m_position.z += abs(blockPointToCheck_SouthBelow.z - playerPointToCheck_SouthBelow.z);
				m_isGrounded = true;
			}
		}
	}

	if (block_WestBelow.GetBlock()) {
		Vector3 playerPointToCheck_WestBelow = m_position + Vector3(0.f, -PLAYER_RADIUS, -(PLAYER_HEIGHT / 2.f));
		Vector3 blockPointToCheck_WestBelow = block_WestBelow.GetBlockWorldPosition() + Vector3(0.f, 1.f, 1.f);

		if (BlockDefinition::IsSolid(block_WestBelow.GetBlock()->GetBlockType())) {

			if (playerPointToCheck_WestBelow.y < blockPointToCheck_WestBelow.y && playerPointToCheck_WestBelow.z < blockPointToCheck_WestBelow.z) {
				m_position.z += abs(blockPointToCheck_WestBelow.z - playerPointToCheck_WestBelow.z);
				m_isGrounded = true;
			}
		}
	}

	if (block_EastBelow.GetBlock()) {
		Vector3 playerPointToCheck_EastBelow = m_position + Vector3(0.f, PLAYER_RADIUS, -(PLAYER_HEIGHT / 2.f));
		Vector3 blockPointToCheck_EastBelow = block_EastBelow.GetBlockWorldPosition() + Vector3(0.f, 0.f, 1.f);

		if (BlockDefinition::IsSolid(block_EastBelow.GetBlock()->GetBlockType())) {

			if (playerPointToCheck_EastBelow.y > blockPointToCheck_EastBelow.y && playerPointToCheck_EastBelow.z < blockPointToCheck_EastBelow.z) {
				m_position.z += abs(blockPointToCheck_EastBelow.z - playerPointToCheck_EastBelow.z);
				m_isGrounded = true;
			}
		}
	}
}

void ThePlayer::CheckAndPushFromMiddleCornerBlocks() {
	//Decrease the problem down to a 2D point-in-disk
	BlockInfo block_NorthEast = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockToNorth().GetBlockToEast();
	BlockInfo block_SouthEast = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockToSouth().GetBlockToEast();
	BlockInfo block_SouthWest = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockToSouth().GetBlockToWest();
	BlockInfo block_NorthWest = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockToNorth().GetBlockToWest();

	Vector2 m_position2D = Vector2(m_position.x, m_position.y);

	if (block_NorthEast.GetBlock()) {
		Vector2	blockPoint_NorthEast = Vector2(block_NorthEast.GetBlockWorldPosition().x + 1.f, block_NorthEast.GetBlockWorldPosition().y);

		if (block_NorthEast.GetBlock() && BlockDefinition::IsSolid(block_NorthEast.GetBlock()->GetBlockType()) && IsPointInDisk(blockPoint_NorthEast, m_position2D, PLAYER_RADIUS))
			PushPlayerFromPoint(blockPoint_NorthEast);
	}

	if (block_SouthEast.GetBlock()) {
		Vector2	blockPoint_SouthEast = Vector2(block_SouthEast.GetBlockWorldPosition().x, block_SouthEast.GetBlockWorldPosition().y);

		if (block_SouthEast.GetBlock() && BlockDefinition::IsSolid(block_SouthEast.GetBlock()->GetBlockType()) && IsPointInDisk(blockPoint_SouthEast, m_position2D, PLAYER_RADIUS))
			PushPlayerFromPoint(blockPoint_SouthEast);
	}

	if (block_SouthWest.GetBlock()) {
		Vector2	blockPoint_SouthWest = Vector2(block_SouthWest.GetBlockWorldPosition().x, block_SouthWest.GetBlockWorldPosition().y + 1.f);

		if (block_SouthWest.GetBlock() && BlockDefinition::IsSolid(block_SouthWest.GetBlock()->GetBlockType()) && IsPointInDisk(blockPoint_SouthWest, m_position2D, PLAYER_RADIUS))
			PushPlayerFromPoint(blockPoint_SouthWest);
	}

	if (block_NorthWest.GetBlock()) {
		Vector2	blockPoint_NorthWest = Vector2(block_NorthWest.GetBlockWorldPosition().x + 1.f, block_NorthWest.GetBlockWorldPosition().y + 1.f);

		if (block_NorthWest.GetBlock() && BlockDefinition::IsSolid(block_NorthWest.GetBlock()->GetBlockType()) && IsPointInDisk(blockPoint_NorthWest, m_position2D, PLAYER_RADIUS))
			PushPlayerFromPoint(blockPoint_NorthWest);
	}
}
void ThePlayer::CheckAndPushFromAboveCornerBlocks() {
	//Decrease the problem down to a 2D point-in-disk
	BlockInfo block_NorthWest = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockAbove().GetBlockToNorth().GetBlockToWest();
	BlockInfo block_NorthEast = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockAbove().GetBlockToNorth().GetBlockToEast();
	BlockInfo block_SouthEast = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockAbove().GetBlockToSouth().GetBlockToEast();
	BlockInfo block_SouthWest = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockAbove().GetBlockToSouth().GetBlockToWest();

	Vector2 m_position2D = Vector2(m_position.x, m_position.y);

	if (block_NorthEast.GetBlock()) {
		Vector2	blockPoint_NorthEast = Vector2(block_NorthEast.GetBlockWorldPosition().x + 1.f, block_NorthEast.GetBlockWorldPosition().y);

		if (block_NorthEast.GetBlock() && BlockDefinition::IsSolid(block_NorthEast.GetBlock()->GetBlockType()) && IsPointInDisk(blockPoint_NorthEast, m_position2D, PLAYER_RADIUS))
			PushPlayerFromPoint(blockPoint_NorthEast);
	}

	if (block_SouthEast.GetBlock()) {
		Vector2	blockPoint_SouthEast = Vector2(block_SouthEast.GetBlockWorldPosition().x, block_SouthEast.GetBlockWorldPosition().y);

		if (block_SouthEast.GetBlock() && BlockDefinition::IsSolid(block_SouthEast.GetBlock()->GetBlockType()) && IsPointInDisk(blockPoint_SouthEast, m_position2D, PLAYER_RADIUS))
			PushPlayerFromPoint(blockPoint_SouthEast);
	}

	if (block_SouthWest.GetBlock()) {
		Vector2	blockPoint_SouthWest = Vector2(block_SouthWest.GetBlockWorldPosition().x, block_SouthWest.GetBlockWorldPosition().y + 1.f);

		if (block_SouthWest.GetBlock() && BlockDefinition::IsSolid(block_SouthWest.GetBlock()->GetBlockType()) && IsPointInDisk(blockPoint_SouthWest, m_position2D, PLAYER_RADIUS))
			PushPlayerFromPoint(blockPoint_SouthWest);
	}

	if (block_NorthWest.GetBlock()) {
		Vector2	blockPoint_NorthWest = Vector2(block_NorthWest.GetBlockWorldPosition().x + 1.f, block_NorthWest.GetBlockWorldPosition().y + 1.f);

		if (block_NorthWest.GetBlock() && BlockDefinition::IsSolid(block_NorthWest.GetBlock()->GetBlockType()) && IsPointInDisk(blockPoint_NorthWest, m_position2D, PLAYER_RADIUS))
			PushPlayerFromPoint(blockPoint_NorthWest);
	}
}
void ThePlayer::CheckAndPushFromBelowCornerBlocks() {

	BlockInfo block_NorthEast = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockBelow().GetBlockToNorth().GetBlockToEast();
	BlockInfo block_SouthEast = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockBelow().GetBlockToSouth().GetBlockToEast();
	BlockInfo block_SouthWest = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockBelow().GetBlockToSouth().GetBlockToWest();
	BlockInfo block_NorthWest = g_theWorld->GetBlockInfoForWorldPosition(m_position).GetBlockBelow().GetBlockToNorth().GetBlockToWest();

	Vector2 m_position2D = Vector2(m_position.x, m_position.y);

	if (block_NorthEast.GetBlock()) {
		Vector3 playerPoint_NorthEast = m_position + Vector3(-PLAYER_RADIUS, 0.f, -(PLAYER_HEIGHT / 2.f));
		Vector3 blockPoint_NorthEast = block_NorthEast.GetBlockWorldPosition() + Vector3(1.f, 0.f, 1.f);

		if (block_NorthEast.GetBlockBelow().GetBlock() && BlockDefinition::IsSolid(block_NorthEast.GetBlock()->GetBlockType())) { 
			if (playerPoint_NorthEast.z < blockPoint_NorthEast.z && IsPointInDisk(Vector2(blockPoint_NorthEast.x, blockPoint_NorthEast.y), m_position2D, PLAYER_RADIUS)) {
				m_position.z += abs(playerPoint_NorthEast.z - blockPoint_NorthEast.z);
				m_isGrounded = true;
			}
		}
	}

	if (block_SouthEast.GetBlock()) {
		Vector3 playerPoint_SouthEast = m_position + Vector3(PLAYER_RADIUS, 0.f, -(PLAYER_HEIGHT / 2.f));
		Vector3 blockPoint_SouthEast = block_SouthEast.GetBlockWorldPosition() + Vector3(0.f, 0.f, 1.f);

		if (block_SouthEast.GetBlockBelow().GetBlock() && BlockDefinition::IsSolid(block_SouthEast.GetBlock()->GetBlockType())) {
			if (playerPoint_SouthEast.z < blockPoint_SouthEast.z && IsPointInDisk(Vector2(blockPoint_SouthEast.x, blockPoint_SouthEast.y), m_position2D, PLAYER_RADIUS)) {
				m_position.z += abs(playerPoint_SouthEast.z - blockPoint_SouthEast.z);
				m_isGrounded = true;
			}
		}
	}

	if (block_SouthWest.GetBlock()) {
		Vector3 playerPoint_SouthWest = m_position + Vector3(0.f, -PLAYER_RADIUS, -(PLAYER_HEIGHT / 2.f));
		Vector3 blockPoint_SouthWest = block_SouthWest.GetBlockWorldPosition() + Vector3(0.f, 1.f, 1.f);

		if (block_SouthWest.GetBlockBelow().GetBlock() && BlockDefinition::IsSolid(block_SouthWest.GetBlock()->GetBlockType())) {
			if (playerPoint_SouthWest.z < blockPoint_SouthWest.z && IsPointInDisk(Vector2(blockPoint_SouthWest.x, blockPoint_SouthWest.y), m_position2D, PLAYER_RADIUS)) {
				m_position.z += abs(playerPoint_SouthWest.z - blockPoint_SouthWest.z);
				m_isGrounded = true;
			}
		}
	}

	if (block_NorthWest.GetBlock()) {
		Vector3 playerPoint_NorthWest = m_position + Vector3(0.f, PLAYER_RADIUS, -(PLAYER_HEIGHT / 2.f));
		Vector3 blockPoint_NorthWest = block_NorthWest.GetBlockWorldPosition() + Vector3(1.f, 1.f, 1.f);

		if (block_NorthWest.GetBlockBelow().GetBlock() && BlockDefinition::IsSolid(block_NorthWest.GetBlock()->GetBlockType())) {
			if (playerPoint_NorthWest.z < blockPoint_NorthWest.z && IsPointInDisk(Vector2(blockPoint_NorthWest.x, blockPoint_NorthWest.y), m_position2D, PLAYER_RADIUS)) {
				m_position.z += abs(playerPoint_NorthWest.z - blockPoint_NorthWest.z);
				m_isGrounded = true;
			}
		}
	}
}

//COLLISION UTILITY
void ThePlayer::PushPlayerFromPoint(const Vector2& pointOnBlock) {
	Vector2 m_position2D = Vector2(m_position.x, m_position.y);
	Vector2 pushDir = m_position2D - pointOnBlock;

	pushDir.normalize();

	float dist = DistanceBetweenPoints(pointOnBlock, m_position2D);
 	pushDir = pushDir * (PLAYER_RADIUS - dist);

	m_position += Vector3(pushDir.x, pushDir.y, 0.f);
}