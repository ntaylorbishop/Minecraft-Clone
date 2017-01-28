#include "Game/TheGame.hpp"

TheGame* g_theGame = nullptr;

//STRUCTORS------------------------------------------------------------------------------------------------------------------------------------------------
TheGame::TheGame() 
    : m_bgTint(RGBA(.2f, .2f, .2f, 1.f))
	, m_middleOfScreen()
	, m_whichBlockTypeSelectedInHUD(-1)
	, m_spriteSheet(nullptr)
	, m_blockBreakAge(0.f)
	, m_mouseBtnDownLastFrame(false)
	, m_mouseBtnDownThisFrame(false)
	, m_currentlyBreakingABlock(false)
{
	m_squirrelFont = BitmapFont::CreateOrGetFont("SquirrelFixedFont");
	m_spriteSheet = SpriteSheet::CreateOrGetSpriteSheet(IntVector2(10, 8), SPRITESHEET_TO_USE);
	m_middleOfScreen = g_theApp->getScreenDimensions() / 2;
	srand((unsigned int)time(NULL)); //Seed RNG

	InitializeCamera();
	InitializePlayer();
	InitializeWorld();
	g_theAudioPlayer = new AudioPlayer();

	g_theAudioPlayer->PlayMusic(2.f);
}
TheGame::~TheGame() {
	delete g_theAudioPlayer;
	g_theAudioPlayer = nullptr;
	delete g_theWorld;
	g_theWorld = nullptr;
	delete g_thePlayer;
	g_thePlayer = nullptr;
	delete g_theCamera;
	g_theCamera = nullptr;
}

//INITIALIZATION------------------------------------------------------------------------------------------------------------------
void TheGame::InitializePlayer() {
	g_thePlayer = new ThePlayer(Vector3(-4.f, 50.f, 90.f), Vector3(0.f, 50.f, 0.f), MOVEMENT_MODE_WALKING);
}
void TheGame::InitializeCamera() {
	g_theCamera = new TheCamera();
}
void TheGame::InitializeWorld() {
	g_theWorld = new TheWorld();
}


//UPDATE--------------------------------------------------------------------------------------------------------------------------------------------
void TheGame::Update(float deltaSeconds) {
	if (g_theApp->hasFocus()) {
		HandlePlayerLook();
		HandlePlayerMovement(deltaSeconds);
		HandlePlayerBlockSelection();
		HandlePlayerBlockPlacementAndDeletion(deltaSeconds);

		HandleCameraModeCycling();
		HandleMovementModeCycling();
	}

	g_theWorld->Update(g_thePlayer->m_position, deltaSeconds);
	g_thePlayer->Update(deltaSeconds);
	g_theCamera->Update();
	g_theAudioPlayer->Update(deltaSeconds);

	SelectBlock();
}

//USER INPUT--------------------------------------------------------------------------------------------------------------------------------------------
void TheGame::HandlePlayerLook() {

	ScreenCoords cursorPos = g_theInputSystem->GetCursorPosition();
	m_cursorDelta = cursorPos - m_middleOfScreen;
	g_theInputSystem->SetCursorPosition(m_middleOfScreen);
	g_theInputSystem->HideMouseCursor();

	g_thePlayer->m_pitchAboutYDeg += (float)m_cursorDelta.y * MOUSE_SENSITIVITY;
	g_thePlayer->m_yawAboutZ -= (float)m_cursorDelta.x * MOUSE_SENSITIVITY;
}
void TheGame::HandlePlayerMovement(float deltaSeconds) {
	deltaSeconds = deltaSeconds;

	Vector3 acceleration_ForwardComponent = Vector3::ZERO;
	Vector3 acceleration_StrafeComponent = Vector3::ZERO;
	Vector3 acceleration_VerticalComponent = Vector3::ZERO;

	//Forward / backward
	if (g_theInputSystem->GetKey('E')) {
		if (g_thePlayer->GetMovementMode() != MOVEMENT_MODE_NOCLIP)
			acceleration_ForwardComponent = g_thePlayer->GetForwardXY() * PLAYER_MOVE_SPEED_WALKING; //Forward
		else
			acceleration_ForwardComponent = g_thePlayer->GetForwardXY() * PLAYER_MOVE_SPEED_NOCLIP; //Forward
	}
	else if (g_theInputSystem->GetKey('D')) {
		if (g_thePlayer->GetMovementMode() != MOVEMENT_MODE_NOCLIP)
			acceleration_ForwardComponent = -1.f * g_thePlayer->GetForwardXY() * PLAYER_MOVE_SPEED_WALKING; //Backward
		else
			acceleration_ForwardComponent = -1.f * g_thePlayer->GetForwardXY() * PLAYER_MOVE_SPEED_NOCLIP; //Backward
	}

	//Left / right
	if (g_theInputSystem->GetKey('S')) {
		if (g_thePlayer->GetMovementMode() != MOVEMENT_MODE_NOCLIP)
			acceleration_StrafeComponent = g_thePlayer->GetLeftXY() * PLAYER_MOVE_SPEED_WALKING; //Left
		else
			acceleration_StrafeComponent = g_thePlayer->GetLeftXY() * PLAYER_MOVE_SPEED_NOCLIP; //Left

	}
	else if (g_theInputSystem->GetKey('F')) {
		if (g_thePlayer->GetMovementMode() != MOVEMENT_MODE_NOCLIP)
			acceleration_StrafeComponent = -1.f * g_thePlayer->GetLeftXY() * PLAYER_MOVE_SPEED_WALKING; //Right
		else
			acceleration_StrafeComponent = -1.f * g_thePlayer->GetLeftXY() * PLAYER_MOVE_SPEED_NOCLIP; //Right
	}

	//Up / Down
	if (g_thePlayer->GetMovementMode() != MOVEMENT_MODE_WALKING) {
		if (g_theInputSystem->GetKey(VK_SPACE)) {
			acceleration_VerticalComponent = Vector3(0.f, 0.f, 1.f) * PLAYER_MOVE_SPEED_WALKING; //Up
		}
		else if (g_theInputSystem->GetKey('C')) {
			acceleration_VerticalComponent = Vector3(0.f, 0.f, -1.f) * PLAYER_MOVE_SPEED_WALKING; //Down
		}
	}

	if (g_thePlayer->GetMovementMode() == MOVEMENT_MODE_WALKING) {
		if (g_theInputSystem->GetKey(VK_SHIFT)) {
			g_thePlayer->IsRunning(true);
		}
		else {
			g_thePlayer->IsRunning(false);
		}
	}

	g_thePlayer->m_acceleration = acceleration_ForwardComponent + acceleration_StrafeComponent + acceleration_VerticalComponent;

	if (g_thePlayer->GetMovementMode() == MOVEMENT_MODE_WALKING && g_theInputSystem->GetKeyDown(VK_SPACE)) {
  		g_thePlayer->Jump();
	}
}
void TheGame::HandlePlayerBlockSelection() {

	if (g_theInputSystem->GetKeyDown(VK_LEFT) || g_theInputSystem->GetMouseWheelUp()) {
		m_whichBlockTypeSelectedInHUD--;

		g_theAudioPlayer->PlaySwitchBlock();

		if (m_whichBlockTypeSelectedInHUD == -1)
			m_whichBlockTypeSelectedInHUD = 4;
	}
	else if (g_theInputSystem->GetKeyDown(VK_RIGHT) || g_theInputSystem->GetMouseWheelDown()) {
		m_whichBlockTypeSelectedInHUD++;

		g_theAudioPlayer->PlaySwitchBlock();

		if (m_whichBlockTypeSelectedInHUD == 5)
			m_whichBlockTypeSelectedInHUD = 0;
	}
}
void TheGame::HandlePlayerBlockPlacementAndDeletion(float deltaSeconds) {

	if (g_theInputSystem->GetMouseBtnDown(MOUSE_RIGHT))
		PlaceBlock();
	else if (g_theInputSystem->GetMouseBtn(MOUSE_LEFT)) {
		BreakBlock(deltaSeconds);
	}
	else if (!g_theInputSystem->GetMouseBtn(MOUSE_LEFT)) {
		ClearBreakBlock();
	}
}

void TheGame::HandleCameraModeCycling() {
	if (g_theInputSystem->GetKeyDown('A')) {
		if (g_theCamera->GetCameraMode() == CAMERA_MODE_FIRST_PERSON) {
			g_theCamera->SetCameraMode(CAMERA_MODE_THIRD_PERSON);
		}
		else if (g_theCamera->GetCameraMode() == CAMERA_MODE_THIRD_PERSON) {
			g_theCamera->SetCameraMode(CAMERA_MODE_SPECTATOR);
		}
		else if (g_theCamera->GetCameraMode() == CAMERA_MODE_SPECTATOR) {
			g_theCamera->SetCameraMode(CAMERA_MODE_FIRST_PERSON);
		}
	}
}
void TheGame::HandleMovementModeCycling() {
	if (g_theInputSystem->GetKeyDown('G')) {
		if (g_thePlayer->GetMovementMode() == MOVEMENT_MODE_FLYING) {
			g_thePlayer->SetMovementMode(MOVEMENT_MODE_WALKING);
		}
		else if (g_thePlayer->GetMovementMode() == MOVEMENT_MODE_WALKING) {
			g_thePlayer->SetMovementMode(MOVEMENT_MODE_NOCLIP);
		}
		else if (g_thePlayer->GetMovementMode() == MOVEMENT_MODE_NOCLIP) {
			g_thePlayer->SetMovementMode(MOVEMENT_MODE_FLYING);
		}
	}
}

//RENDER--------------------------------------------------------------------------------------------------------------------------------------------
void TheGame::SetUpPerspectiveProjection() const {
	float aspect = 16.f / 9.f;

	float fovDegreesVertical;
	if (!g_isRunning)
		fovDegreesVertical = 70.f;
	else
		fovDegreesVertical = 90.f;

	float nearClippingPlane = 0.1f;
	float farClippingPlane = 1000.f;

	g_theRenderer->SetPerspective(fovDegreesVertical, aspect, nearClippingPlane, farClippingPlane);

	g_theRenderer->Enable(GL_CULL_FACE); //Enable backface culling
	g_theRenderer->Enable(GL_DEPTH_TEST); //Enable depth rendering
	g_theRenderer->Enable(GL_ALPHA_TEST);

	//Fix coordinate system
	g_theRenderer->RotateViewMatrix(-90.f, Vector3(1.f, 0.f, 0.f)); //Put z up
	g_theRenderer->RotateViewMatrix(90.f, Vector3(0.f, 0.f, 1.f)); //Put x forward

	g_theRenderer->RotateViewMatrix(-g_theCamera->m_rollAboutX, Vector3(1.f, 0.f, 0.f));
	g_theRenderer->RotateViewMatrix(-g_theCamera->m_pitchAboutYDeg, Vector3(0.f, 1.f, 0.f));
	g_theRenderer->RotateViewMatrix(-g_theCamera->m_yawAboutZ, Vector3(0.f, 0.f, 1.f));

	g_theRenderer->TranslateViewMatrix(Vector3(-g_theCamera->m_position.x, -g_theCamera->m_position.y, -g_theCamera->m_position.z));
}
void TheGame::Render() const {
	RGBA bgColor = RGBA(0x00, 0x59, 0x87, 0xff);
	g_theRenderer->ClearScreen(g_skyColor.r(), g_skyColor.g(), g_skyColor.b());

	//Render 3Dg_isRunning
	SetUpPerspectiveProjection();
	g_theWorld->Render(g_theCamera->GetForwardXYZ(), g_thePlayer->m_position);
	g_thePlayer->Render();

	RenderBlockSelection();
	RenderCracksVertexArray();

	//Render 2D
	RenderAxis();
	if(DEBUG_EnableDebugDrawing)
		RenderDebugText();
	RenderHUD();
}

void TheGame::RenderBlockSelection() const {
	if (m_blockIsSelected) {
		BlockInfo currBlock = TheWorld::GetBlockInfoForWorldPosition(m_selectedBlockPos);
		RGBA selectionTint = RGBA(0.1f, 0.1f, 0.1f, 1.f);
		g_theRenderer->Disable(GL_DEPTH_TEST);
		g_theRenderer->LineWidth(1.f);

		BlockType currBlockType = BLOCK_NUM_TYPES;
		if (currBlock.GetBlock()) {
			currBlockType = currBlock.GetBlock()->GetBlockType();
		}

		if (currBlockType != BLOCK_SNOWTOP) {
			if (m_selectedFace == FACE_NORTH) {
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 0.f, 0.f), m_selectedBlockPos + Vector3(0.f, 1.f, 0.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 1.f, 0.f), m_selectedBlockPos + Vector3(0.f, 1.f, 1.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 1.f, 1.f), m_selectedBlockPos + Vector3(0.f, 0.f, 1.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 0.f, 1.f), m_selectedBlockPos + Vector3(0.f, 0.f, 0.f), selectionTint);
			}
			else if (m_selectedFace == FACE_SOUTH) {
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 0.f, 0.f), m_selectedBlockPos + Vector3(1.f, 1.f, 0.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 1.f, 0.f), m_selectedBlockPos + Vector3(1.f, 1.f, 1.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 1.f, 1.f), m_selectedBlockPos + Vector3(1.f, 0.f, 1.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 0.f, 1.f), m_selectedBlockPos + Vector3(1.f, 0.f, 0.f), selectionTint);
			}
			else if (m_selectedFace == FACE_WEST) {
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 0.f, 0.f), m_selectedBlockPos + Vector3(0.f, 0.f, 1.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 0.f, 1.f), m_selectedBlockPos + Vector3(1.f, 0.f, 1.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 0.f, 1.f), m_selectedBlockPos + Vector3(1.f, 0.f, 0.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 0.f, 0.f), m_selectedBlockPos + Vector3(0.f, 0.f, 0.f), selectionTint);
			}
			else if (m_selectedFace == FACE_EAST) {
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 1.f, 0.f), m_selectedBlockPos + Vector3(0.f, 1.f, 0.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 1.f, 0.f), m_selectedBlockPos + Vector3(0.f, 1.f, 1.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 1.f, 1.f), m_selectedBlockPos + Vector3(1.f, 1.f, 1.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 1.f, 1.f), m_selectedBlockPos + Vector3(1.f, 1.f, 0.f), selectionTint);
			}
			else if (m_selectedFace == FACE_TOP) {
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 0.f, 1.f), m_selectedBlockPos + Vector3(0.f, 1.f, 1.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 1.f, 1.f), m_selectedBlockPos + Vector3(1.f, 1.f, 1.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 1.f, 1.f), m_selectedBlockPos + Vector3(1.f, 0.f, 1.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 0.f, 1.f), m_selectedBlockPos + Vector3(0.f, 0.f, 1.f), selectionTint);
			}
			else if (m_selectedFace == FACE_BOTTOM) {
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 0.f, 0.f), m_selectedBlockPos + Vector3(0.f, 1.f, 0.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 1.f, 0.f), m_selectedBlockPos + Vector3(1.f, 1.f, 0.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 1.f, 0.f), m_selectedBlockPos + Vector3(1.f, 0.f, 0.f), selectionTint);
				g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 0.f, 0.f), m_selectedBlockPos + Vector3(0.f, 0.f, 0.f), selectionTint);
			}
		}
		else {
			//North face
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 0.f, 0.f), m_selectedBlockPos + Vector3(0.f, 1.f, 0.f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 1.f, 0.f), m_selectedBlockPos + Vector3(0.f, 1.f, 0.2f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 1.f, 0.2f), m_selectedBlockPos + Vector3(0.f, 0.f, 0.2f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 0.f, 0.2f), m_selectedBlockPos + Vector3(0.f, 0.f, 0.f), selectionTint);
			//South face
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 0.f, 0.f), m_selectedBlockPos + Vector3(1.f, 1.f, 0.f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 1.f, 0.f), m_selectedBlockPos + Vector3(1.f, 1.f, 0.2f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 1.f, 0.2f), m_selectedBlockPos + Vector3(1.f, 0.f, 0.2f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 0.f, 0.2f), m_selectedBlockPos + Vector3(1.f, 0.f, 0.f), selectionTint);
			//West face
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 0.f, 0.f), m_selectedBlockPos + Vector3(0.f, 0.f, 0.2f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 0.f, 0.2f), m_selectedBlockPos + Vector3(1.f, 0.f, 0.2f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 0.f, 0.2f), m_selectedBlockPos + Vector3(1.f, 0.f, 0.f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 0.f, 0.f), m_selectedBlockPos + Vector3(0.f, 0.f, 0.f), selectionTint);
			//East face
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 1.f, 0.f), m_selectedBlockPos + Vector3(0.f, 1.f, 0.f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 1.f, 0.f), m_selectedBlockPos + Vector3(0.f, 1.f, 0.2f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 1.f, 0.2f), m_selectedBlockPos + Vector3(1.f, 1.f, 0.2f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 1.f, 0.2f), m_selectedBlockPos + Vector3(1.f, 1.f, 0.f), selectionTint);
			//Top face
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 0.f, 0.2f), m_selectedBlockPos + Vector3(0.f, 1.f, 0.2f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 1.f, 0.2f), m_selectedBlockPos + Vector3(1.f, 1.f, 0.2f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 1.f, 0.2f), m_selectedBlockPos + Vector3(1.f, 0.f, 0.2f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 0.f, 0.2f), m_selectedBlockPos + Vector3(0.f, 0.f, 0.2f), selectionTint);
			//Bottom face
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 0.f, 0.f), m_selectedBlockPos + Vector3(0.f, 1.f, 0.f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(0.f, 1.f, 0.f), m_selectedBlockPos + Vector3(1.f, 1.f, 0.f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 1.f, 0.f), m_selectedBlockPos + Vector3(1.f, 0.f, 0.f), selectionTint);
			g_theRenderer->DrawLine(m_selectedBlockPos + Vector3(1.f, 0.f, 0.f), m_selectedBlockPos + Vector3(0.f, 0.f, 0.f), selectionTint);
		}
	}
}

//DEBUG HUD--------------------------------------------------------------------------------------------------------------------------------------------
void TheGame::RenderDebugText() const {
	g_theRenderer->SetOrtho2D(Vector2(0.f, 0.f), Vector2(1600.f, 900.f));

	RGBA debugTint = RGBA(1.f, 1.f, 0.f, 1.f);

	std::string velocity = Stringf("Position: (%3.2f, %3.2f, %3.2f)", g_thePlayer->m_position.x, g_thePlayer->m_position.y, g_thePlayer->m_position.z);

	m_squirrelFont->DrawText2D(Vector2(10.f, 10.f), velocity, 10.f, debugTint, NULL);
}
void TheGame::RenderAxis() const {

	g_theRenderer->Enable(GL_DEPTH_TEST);
	g_theRenderer->LineWidth(7.f);

	g_theRenderer->DrawLine(Vector3(0.f, 0.f, 0.f), Vector3(10.f, 0.f, 0.f), RGBA(1.f, 0.f, 0.f, 0.5f)); //Red X Axis
	g_theRenderer->DrawLine(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 10.f, 0.f), RGBA(0.f, 1.f, 0.f, 0.5f)); //Green Y Axis
	g_theRenderer->DrawLine(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 10.f), RGBA(0.f, 0.f, 1.f, 0.5f)); //Blue Z Axis

	g_theRenderer->Disable(GL_DEPTH_TEST);
	g_theRenderer->LineWidth(3.f);

	g_theRenderer->DrawLine(Vector3(0.f, 0.f, 0.f), Vector3(10.f, 0.f, 0.f), RGBA(1.f, 0.f, 0.f, 0.5f)); //Red X Axis
	g_theRenderer->DrawLine(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 10.f, 0.f), RGBA(0.f, 1.f, 0.f, 0.5f)); //Green Y Axis
	g_theRenderer->DrawLine(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 10.f), RGBA(0.f, 0.f, 1.f, 0.5f)); //Blue Z Axis
}

//HUD--------------------------------------------------------------------------------------------------------------------------------------------
void TheGame::RenderHUD() const {
	g_theRenderer->SetOrtho2D(Vector2(0.f, 0.f), Vector2(1600.f, 900.f));

	RenderCrosshair();
	RenderBlockSelectionUI();
	RenderCameraAndMovementModeSelection();
	RenderCurrentBiome();
}
void TheGame::RenderCrosshair() const {
	g_theRenderer->BlendMode(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
	g_theRenderer->SetColor(RGBA(1.f, 1.f, 1.f, 1.f));
	g_theRenderer->DrawLine(Vector3(800.f, 440.f, 0.f), Vector3(800.f, 460.f, 0.f), RGBA::WHITE);
	g_theRenderer->DrawLine(Vector3(790.f, 450.f, 0.f), Vector3(810.f, 450.f, 0.f), RGBA::WHITE);
	g_theRenderer->BlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void TheGame::RenderBlockSelectionUI() const {
	//Draw block to use
	Vector2 HUDBlock1Position = Vector2(700.f, 100.f);
	Vector2 HUDBlock2Position = Vector2(760.f, 100.f);
	Vector2 HUDBlock3Position = Vector2(820.f, 100.f);
	Vector2 HUDBlock4Position = Vector2(880.f, 100.f);
	Vector2 HUDBlock5Position = Vector2(940.f, 100.f);
	Vector2 HUDBlockSize = Vector2(50.f, 50.f);

	RGBA HUDBlock1Tint = RGBA(0.f, 0.f, 0.f, 1.f);
	RGBA HUDBlock2Tint = RGBA(0.f, 0.f, 0.f, 1.f);
	RGBA HUDBlock3Tint = RGBA(0.f, 0.f, 0.f, 1.f);
	RGBA HUDBlock4Tint = RGBA(0.f, 0.f, 0.f, 1.f);
	RGBA HUDBlock5Tint = RGBA(0.f, 0.f, 0.f, 1.f);

	switch (m_whichBlockTypeSelectedInHUD) {
		case 0: {
			HUDBlock1Tint = RGBA(1.f, 1.f, 1.f, 1.f);
			break;
		}
		case 1: {
			HUDBlock2Tint = RGBA(1.f, 1.f, 1.f, 1.f);
			break;
		}
		case 2: {
			HUDBlock3Tint = RGBA(1.f, 1.f, 1.f, 1.f);
			break;
		}
		case 3: {
			HUDBlock4Tint = RGBA(1.f, 1.f, 1.f, 1.f);
			break;
		}
		case 4: {
			HUDBlock5Tint = RGBA(1.f, 1.f, 1.f, 1.f);
			break;
		}
	}

	g_theRenderer->DrawAABB2(AABB2(HUDBlock1Position - Vector2(4.f, 4.f), HUDBlock1Position + HUDBlockSize + Vector2(4.f, 4.f)), HUDBlock1Tint, PRIMITIVE_QUADS);
	g_theRenderer->DrawAABB2(AABB2(HUDBlock2Position - Vector2(4.f, 4.f), HUDBlock2Position + HUDBlockSize + Vector2(4.f, 4.f)), HUDBlock2Tint, PRIMITIVE_QUADS);
	g_theRenderer->DrawAABB2(AABB2(HUDBlock3Position - Vector2(4.f, 4.f), HUDBlock3Position + HUDBlockSize + Vector2(4.f, 4.f)), HUDBlock3Tint, PRIMITIVE_QUADS);
	g_theRenderer->DrawAABB2(AABB2(HUDBlock4Position - Vector2(4.f, 4.f), HUDBlock4Position + HUDBlockSize + Vector2(4.f, 4.f)), HUDBlock4Tint, PRIMITIVE_QUADS);
	g_theRenderer->DrawAABB2(AABB2(HUDBlock5Position - Vector2(4.f, 4.f), HUDBlock5Position + HUDBlockSize + Vector2(4.f, 4.f)), HUDBlock5Tint, PRIMITIVE_QUADS);

	g_theRenderer->DrawTexturedAABB2FromSpriteSheet(m_spriteSheet, RGBA(1.f, 1.f, 1.f, 1.f), AABB2(HUDBlock1Position, HUDBlock1Position + HUDBlockSize), BlockDefinition::SpriteSides(BLOCK_GRASS));
	g_theRenderer->DrawTexturedAABB2FromSpriteSheet(m_spriteSheet, RGBA(1.f, 1.f, 1.f, 1.f), AABB2(HUDBlock2Position, HUDBlock2Position + HUDBlockSize), BlockDefinition::SpriteSides(BLOCK_COBBLESTONE));
	g_theRenderer->DrawTexturedAABB2FromSpriteSheet(m_spriteSheet, RGBA(1.f, 1.f, 1.f, 1.f), AABB2(HUDBlock3Position, HUDBlock3Position + HUDBlockSize), BlockDefinition::SpriteSides(BLOCK_GLOWSTONE));
	g_theRenderer->DrawTexturedAABB2FromSpriteSheet(m_spriteSheet, RGBA(1.f, 1.f, 1.f, 1.f), AABB2(HUDBlock4Position, HUDBlock4Position + HUDBlockSize), BlockDefinition::SpriteSides(BLOCK_SAND));
	g_theRenderer->DrawTexturedAABB2FromSpriteSheet(m_spriteSheet, RGBA(1.f, 1.f, 1.f, 1.f), AABB2(HUDBlock5Position, HUDBlock5Position + HUDBlockSize), BlockDefinition::SpriteSides(BLOCK_STONE));
}
void TheGame::RenderCameraAndMovementModeSelection() const {

	string cameraMode = "Camera Mode: " + g_theCamera->GetCameraModeStringified();
	string movementMode = "Movement Mode: " + g_thePlayer->GetMovementModeStringified();

	m_squirrelFont->DrawText2D(Vector2(10.f, 885.f), cameraMode, 10.f, RGBA::WHITE, NULL);
	m_squirrelFont->DrawText2D(Vector2(1200.f, 885.f), movementMode, 10.f, RGBA::WHITE, NULL);
}
void TheGame::RenderCurrentBiome() const {
	WorldPosition playerCenteredOnZ = WorldPosition(g_thePlayer->m_position.x, g_thePlayer->m_position.y, 0.f);
	BlockInfo bi = g_theWorld->GetBlockInfoForWorldPosition(playerCenteredOnZ);
	Chunk* currChunk = bi.chunk;

	string currBiomeStringified;
	if (currChunk) {
		BIOME currBiome = currChunk->GetColumnBiome(playerCenteredOnZ);

		if (currBiome == BIOME_PLAINS)
			currBiomeStringified = "Plains";
		else if (currBiome == BIOME_DESERT)
			currBiomeStringified = "Desert";
		else if (currBiome == BIOME_MOUNTAIN)
			currBiomeStringified = "Mountain";
		else if (currBiome == BIOME_TUNDRA)
			currBiomeStringified = "Tundra";
		else
			currBiomeStringified = "NOT AVAILABLE (LOADED FROM DISK)";
	}

	string currBiomeText = "Current Biome: " + currBiomeStringified;
	m_squirrelFont->DrawText2D(Vector2(1200.f, 10.f), currBiomeText, 10.f, RGBA::WHITE, NULL);
}

//BLOCK PLACEMENT-----------------------------------------------------------------------------------------------------------------------------------
void TheGame::SelectBlock() {
	Vector3 start = g_thePlayer->m_position + Vector3(0.f, 0.f, 0.69f);
	Vector3 end = g_thePlayer->GetForwardDisplacedByValue(PLAYER_SELECTION_RANGE, Vector3(0.f, 0.f, 0.69f));

	RaycastResult raycastFromPlayer;
	Raycast3DToPoint(start, end, &raycastFromPlayer);
	if (raycastFromPlayer.m_didImpact && (raycastFromPlayer.m_impactBlock->GetBlockType() != BLOCK_WATER || raycastFromPlayer.m_impactBlock->GetBlockType() != BLOCK_SNOWTOP)) {
		m_selectedBlockPos = raycastFromPlayer.m_impactPos;
		m_selectedBlockPos = Chunk::GetWorldCoordsForLocalCoords(m_selectedBlockPos);
		m_prevSelectedBlockPos = raycastFromPlayer.m_prevImpactBlockPos;
		m_prevSelectedBlockPos = Chunk::GetWorldCoordsForLocalCoords(m_prevSelectedBlockPos);
		m_selectedFace = raycastFromPlayer.m_impactSurfaceNormal;
		m_blockIsSelected = true;
	}
	else {
		m_blockIsSelected = false;
	}
}
void TheGame::PlaceBlock() {
	WorldPosition newBlockPos = m_prevSelectedBlockPos;
	ChunkCoords chunkToPlaceOnPos = Chunk::GetChunkCoordsForWorldCoords(newBlockPos);

	auto it = g_theWorld->m_activeChunks.find(chunkToPlaceOnPos);
	if (it != g_theWorld->m_activeChunks.end()) {
		Chunk* chunk = it->second;

		if (m_whichBlockTypeSelectedInHUD == 0) {
			chunk->PlaceBlock(newBlockPos, BLOCK_GRASS);
			g_theAudioPlayer->PlayPlaceBlock(BLOCK_GRASS);
		}
		else if (m_whichBlockTypeSelectedInHUD == 1) {
			chunk->PlaceBlock(newBlockPos, BLOCK_COBBLESTONE);
			g_theAudioPlayer->PlayPlaceBlock(BLOCK_COBBLESTONE);
		}
		else if (m_whichBlockTypeSelectedInHUD == 2) {
			chunk->PlaceBlock(newBlockPos, BLOCK_GLOWSTONE);
			g_theAudioPlayer->PlayPlaceBlock(BLOCK_GLOWSTONE);
		}
		else if (m_whichBlockTypeSelectedInHUD == 3) {
			chunk->PlaceBlock(newBlockPos, BLOCK_SAND);
			g_theAudioPlayer->PlayPlaceBlock(BLOCK_SAND);
		}
		else if (m_whichBlockTypeSelectedInHUD == 4) {
			chunk->PlaceBlock(newBlockPos, BLOCK_STONE);
			g_theAudioPlayer->PlayPlaceBlock(BLOCK_STONE);
		}
	}
}

void TheGame::PushCracksToVertexArray(const IntVector2& currCrack, const BlockInfo& blockThatIsBeingBroken) {
	m_cracksInBlock.clear();

	Vector3 blockPos = Chunk::GetWorldPositionOfBlockInChunk(blockThatIsBeingBroken.chunk, Chunk::GetLocalCoordsForBlockIndex(blockThatIsBeingBroken.m_idx));
	
	//FRONT FACE
	Vector3 bottomLeft =  Vector3(-0.001f, 1.f, 0.f) + blockPos;
	Vector3 bottomRight = Vector3(-0.001f, 0.f, 0.f) + blockPos;
	Vector3 topRight =    Vector3(-0.001f, 0.f, 1.f) + blockPos;
	Vector3 topLeft =     Vector3(-0.001f, 1.f, 1.f) + blockPos;

	SpriteCoords spriteCoords = m_spriteSheet->GetTexCoordsForSpriteCoords(currCrack);
	TexCoords bottomLeft_TC = TexCoords(spriteCoords.mins.x, spriteCoords.maxs.y);
	TexCoords bottomRight_TC = TexCoords(spriteCoords.maxs.x, spriteCoords.maxs.y);
	TexCoords topRight_TC = TexCoords(spriteCoords.maxs.x, spriteCoords.mins.y);
	TexCoords topLeft_TC = TexCoords(spriteCoords.mins.x, spriteCoords.mins.y);

	m_cracksInBlock.push_back(TexturedVertex(bottomRight, RGBA(1.f, 1.f, 1.f, 0.5f), bottomRight_TC));
	m_cracksInBlock.push_back(TexturedVertex(topRight, RGBA(1.f, 1.f, 1.f, 0.5f), topRight_TC));
	m_cracksInBlock.push_back(TexturedVertex(topLeft, RGBA(1.f, 1.f, 1.f, 0.5f), topLeft_TC));
	m_cracksInBlock.push_back(TexturedVertex(bottomLeft, RGBA(1.f, 1.f, 1.f, 0.5f), bottomLeft_TC));

	//BACK FACE
	bottomLeft =  Vector3(1.001f, 0.f, 0.f) + blockPos;
	bottomRight = Vector3(1.001f, 1.f, 0.f) + blockPos;
	topRight =    Vector3(1.001f, 1.f, 1.f) + blockPos;
	topLeft =     Vector3(1.001f, 0.f, 1.f) + blockPos;


	spriteCoords = m_spriteSheet->GetTexCoordsForSpriteCoords(currCrack);
	bottomLeft_TC = TexCoords(spriteCoords.mins.x, spriteCoords.maxs.y);
	bottomRight_TC = TexCoords(spriteCoords.maxs.x, spriteCoords.maxs.y);
	topRight_TC = TexCoords(spriteCoords.maxs.x, spriteCoords.mins.y);
	topLeft_TC = TexCoords(spriteCoords.mins.x, spriteCoords.mins.y);

	m_cracksInBlock.push_back(TexturedVertex(bottomRight, RGBA(1.f, 1.f, 1.f, 0.5f), bottomRight_TC));
	m_cracksInBlock.push_back(TexturedVertex(topRight, RGBA(1.f, 1.f, 1.f, 0.5f), topRight_TC));
	m_cracksInBlock.push_back(TexturedVertex(topLeft, RGBA(1.f, 1.f, 1.f, 0.5f), topLeft_TC));
	m_cracksInBlock.push_back(TexturedVertex(bottomLeft, RGBA(1.f, 1.f, 1.f, 0.5f), bottomLeft_TC));

	//BOTTOM FACE
	bottomLeft =  Vector3(0.f, 0.f, -0.001f) + blockPos;
	bottomRight = Vector3(0.f, 1.f, -0.001f) + blockPos;
	topRight =    Vector3(1.f, 1.f, -0.001f) + blockPos;
	topLeft =     Vector3(1.f, 0.f, -0.001f) + blockPos;

	spriteCoords = m_spriteSheet->GetTexCoordsForSpriteCoords(currCrack);
	bottomLeft_TC = TexCoords(spriteCoords.mins.x, spriteCoords.maxs.y);
	bottomRight_TC = TexCoords(spriteCoords.maxs.x, spriteCoords.maxs.y);
	topRight_TC = TexCoords(spriteCoords.maxs.x, spriteCoords.mins.y);
	topLeft_TC = TexCoords(spriteCoords.mins.x, spriteCoords.mins.y);

	m_cracksInBlock.push_back(TexturedVertex(bottomRight, RGBA(1.f, 1.f, 1.f, 0.5f), bottomRight_TC));
	m_cracksInBlock.push_back(TexturedVertex(topRight, RGBA(1.f, 1.f, 1.f, 0.5f), topRight_TC));
	m_cracksInBlock.push_back(TexturedVertex(topLeft, RGBA(1.f, 1.f, 1.f, 0.5f), topLeft_TC));
	m_cracksInBlock.push_back(TexturedVertex(bottomLeft, RGBA(1.f, 1.f, 1.f, 0.5f), bottomLeft_TC));

	//TOP FACE
	bottomLeft =  Vector3(0.f, 1.f, 1.001f) + blockPos;
	bottomRight = Vector3(0.f, 0.f, 1.001f) + blockPos;
	topRight =    Vector3(1.f, 0.f, 1.001f) + blockPos;
	topLeft =     Vector3(1.f, 1.f, 1.001f) + blockPos;

	spriteCoords = m_spriteSheet->GetTexCoordsForSpriteCoords(currCrack);
	bottomLeft_TC = TexCoords(spriteCoords.mins.x, spriteCoords.maxs.y);
	bottomRight_TC = TexCoords(spriteCoords.maxs.x, spriteCoords.maxs.y);
	topRight_TC = TexCoords(spriteCoords.maxs.x, spriteCoords.mins.y);
	topLeft_TC = TexCoords(spriteCoords.mins.x, spriteCoords.mins.y);

	m_cracksInBlock.push_back(TexturedVertex(bottomRight, RGBA(1.f, 1.f, 1.f, 0.5f), bottomRight_TC));
	m_cracksInBlock.push_back(TexturedVertex(topRight, RGBA(1.f, 1.f, 1.f, 0.5f), topRight_TC));
	m_cracksInBlock.push_back(TexturedVertex(topLeft, RGBA(1.f, 1.f, 1.f, 0.5f), topLeft_TC));
	m_cracksInBlock.push_back(TexturedVertex(bottomLeft, RGBA(1.f, 1.f, 1.f, 0.5f), bottomLeft_TC));

	//RIGHT FACE
	bottomLeft =  Vector3(0.f, -0.001f, 0.f) + blockPos;
	bottomRight = Vector3(1.f, -0.001f, 0.f) + blockPos;
	topRight =    Vector3(1.f, -0.001f, 1.f) + blockPos;
	topLeft =     Vector3(0.f, -0.001f, 1.f) + blockPos;

	spriteCoords = m_spriteSheet->GetTexCoordsForSpriteCoords(currCrack);
	bottomLeft_TC = TexCoords(spriteCoords.mins.x, spriteCoords.maxs.y);
	bottomRight_TC = TexCoords(spriteCoords.maxs.x, spriteCoords.maxs.y);
	topRight_TC = TexCoords(spriteCoords.maxs.x, spriteCoords.mins.y);
	topLeft_TC = TexCoords(spriteCoords.mins.x, spriteCoords.mins.y);

	m_cracksInBlock.push_back(TexturedVertex(bottomRight, RGBA(1.f, 1.f, 1.f, 0.5f), bottomRight_TC));
	m_cracksInBlock.push_back(TexturedVertex(topRight, RGBA(1.f, 1.f, 1.f, 0.5f), topRight_TC));
	m_cracksInBlock.push_back(TexturedVertex(topLeft, RGBA(1.f, 1.f, 1.f, 0.5f), topLeft_TC));
	m_cracksInBlock.push_back(TexturedVertex(bottomLeft, RGBA(1.f, 1.f, 1.f, 0.5f), bottomLeft_TC));

	//LEFT FACE
	bottomLeft =  Vector3(1.f, 1.001f, 0.f) + blockPos;
	bottomRight = Vector3(0.f, 1.001f, 0.f) + blockPos;
	topRight =    Vector3(0.f, 1.001f, 1.f) + blockPos;
	topLeft =     Vector3(1.f, 1.001f, 1.f) + blockPos;

	spriteCoords = m_spriteSheet->GetTexCoordsForSpriteCoords(currCrack);
	bottomLeft_TC = TexCoords(spriteCoords.mins.x, spriteCoords.maxs.y);
	bottomRight_TC = TexCoords(spriteCoords.maxs.x, spriteCoords.maxs.y);
	topRight_TC = TexCoords(spriteCoords.maxs.x, spriteCoords.mins.y);
	topLeft_TC = TexCoords(spriteCoords.mins.x, spriteCoords.mins.y);

	m_cracksInBlock.push_back(TexturedVertex(bottomRight, RGBA(1.f, 1.f, 1.f, 0.5f), bottomRight_TC));
	m_cracksInBlock.push_back(TexturedVertex(topRight, RGBA(1.f, 1.f, 1.f, 0.5f), topRight_TC));
	m_cracksInBlock.push_back(TexturedVertex(topLeft, RGBA(1.f, 1.f, 1.f, 0.5f), topLeft_TC));
	m_cracksInBlock.push_back(TexturedVertex(bottomLeft, RGBA(1.f, 1.f, 1.f, 0.5f), bottomLeft_TC));
}
void TheGame::RenderCracksVertexArray() const {
	if (m_cracksInBlock.size() > 0 && m_currentlyBreakingABlock) {
		g_theRenderer->Enable(GL_DEPTH_TEST);
		g_theRenderer->BindAndEnableTexture(const_cast<Texture*>(m_spriteSheet->GetTexture()));
		g_theRenderer->DrawVertexArray(m_cracksInBlock, m_cracksInBlock.size(), PRIMITIVE_QUADS);
		g_theRenderer->UnbindAndDisableTexture(const_cast<Texture*>(m_spriteSheet->GetTexture()));
	}
}
void TheGame::BreakBlock(float deltaSeconds) {
	m_mouseBtnDownLastFrame = m_mouseBtnDownThisFrame;
	m_mouseBtnDownThisFrame = g_theInputSystem->GetMouseBtn(MOUSE_LEFT);
	m_prevBlockSelected = m_currBlockSelected;
	m_currBlockSelected = g_theWorld->GetBlockInfoForWorldPosition(m_selectedBlockPos);

	if (m_blockIsSelected && m_currBlockSelected.chunk && m_prevBlockSelected.chunk) {
		if (m_mouseBtnDownLastFrame != m_mouseBtnDownThisFrame && m_prevBlockSelected != m_currBlockSelected) { //First time right-clicking on a block
			m_blockBreakAge = 0.f;
			m_currentlyBreakingABlock = false;
		}
		else if (m_mouseBtnDownLastFrame == m_mouseBtnDownThisFrame && m_prevBlockSelected != m_currBlockSelected) { //Moved away from currently-selected block
			m_blockBreakAge = 0.f;
			m_currentlyBreakingABlock = false;
		}
		else {
			if (m_currBlockSelected.GetBlock()) { //Currently breaking a block
				if (m_blockBreakAge == 0.f)
					g_theAudioPlayer->PlayBreakBlock(m_currBlockSelected.GetBlock()->GetBlockType());


				m_blockBreakAge += deltaSeconds;
				m_currentlyBreakingABlock = true;

				if (g_thePlayer->GetMovementMode() != MOVEMENT_MODE_NOCLIP) {
					int whichCrackToShow = (int)((m_blockBreakAge / BlockDefinition::GetBlockToughness(m_currBlockSelected.GetBlock()->GetBlockType())) * 10.f);

					if (m_currBlockSelected.GetBlock()->GetBlockType() != BLOCK_SNOWTOP)
						PushCracksToVertexArray(IntVector2(whichCrackToShow, 6), m_currBlockSelected);

					if (m_blockBreakAge >= BlockDefinition::GetBlockToughness(m_currBlockSelected.GetBlock()->GetBlockType()))
						DeleteBlock();
				}
				else {
					if (m_blockBreakAge >= 0.2f)
						DeleteBlock();
				}
			}
		}
	}
}
void TheGame::ClearBreakBlock() {
	m_cracksInBlock.clear();
	m_blockBreakAge = 0.f;
	m_currentlyBreakingABlock = false;
}
void TheGame::DeleteBlock() {
	WorldPosition deletedBlockPos = m_selectedBlockPos;
	ChunkCoords chunkToDeleteFrom = Chunk::GetChunkCoordsForWorldCoords(deletedBlockPos);

	auto it = g_theWorld->m_activeChunks.find(chunkToDeleteFrom);
	if (it != g_theWorld->m_activeChunks.end()) {
		Chunk* chunk = it->second;

		chunk->DeleteBlock(deletedBlockPos);
	}
}