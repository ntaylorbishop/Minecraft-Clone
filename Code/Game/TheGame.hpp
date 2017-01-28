#pragma once

#include <stdlib.h>
#include <map>
#include <string>
#include <utility>
#include <time.h>
#include <stdlib.h>

#include "Engine/Renderer/Vertex.hpp"
#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/VectorUtils.hpp"
#include "Engine/InputSystem.hpp"
#include "Engine/StringUtils.hpp"
#include "Game/TheApp.hpp"
#include "Game/GameCommon.hpp"
#include "Game/TheCamera.hpp"
#include "Game/Chunk.hpp"
#include "Game/ThePlayer.hpp"
#include "Game/TheCamera.hpp"
#include "Game/TheWorld.hpp"
#include "Game/Raycast3D.hpp"
#include "Game/AudioPlayer.hpp"

typedef IntVector2 ScreenCoords;

class TheGame;
extern TheGame* g_theGame;

using namespace std;

class TheGame {
public:
	//STRUCTORS
	TheGame();
	~TheGame();

	//INITIALIZATION
	void Update(float deltaSeconds);

	//USER INPUT
	void HandlePlayerLook();
	void HandlePlayerMovement(float deltaSeconds);
	void InitializePlayer();
	void InitializeCamera();
	void InitializeWorld();

	//UPDATE
	void HandlePlayerBlockSelection();
	void HandlePlayerBlockPlacementAndDeletion(float deltaSeconds);

	void HandleCameraModeCycling();
	void HandleMovementModeCycling();

	//RENDER
	void SetUpPerspectiveProjection() const;
	void Render() const;

	void RenderBlockSelection() const;


	//DEBUG HUD
	void RenderDebugText() const;
	void RenderAxis() const;

	//HUD
	void RenderHUD() const;
	void RenderCrosshair() const;
	void RenderBlockSelectionUI() const;
	void RenderCameraAndMovementModeSelection() const;
	void RenderCurrentBiome() const;

	//BLOCK PLACEMENT
	void SelectBlock();
	void PlaceBlock();

	void PushCracksToVertexArray(const IntVector2& currCrack, const BlockInfo& blockThatIsBeingBroken);
	void RenderCracksVertexArray() const;
	void BreakBlock(float deltaSeconds);
	void ClearBreakBlock();
	void DeleteBlock();

private:
	SpriteSheet* m_spriteSheet;

	//UI
	char* m_fontToUse = "Data/Fonts/SquirrelFixedFont.png";
	RGBA m_bgTint;
	BitmapFont* m_squirrelFont;

	int m_whichBlockTypeSelectedInHUD;

	//MOUSE INPUT
	ScreenCoords m_middleOfScreen;
	ScreenCoords m_cursorDelta;

	//BLOCK SELECTION
	bool m_blockIsSelected;
	Vector3 m_selectedBlockPos;
	Vector3 m_prevSelectedBlockPos;
	BlockFace m_selectedFace;

	//BLOCK BREAKING
	vector<TexturedVertex> m_cracksInBlock;
	bool m_mouseBtnDownLastFrame;
	bool m_mouseBtnDownThisFrame;
	
	BlockInfo m_currBlockSelected;
	BlockInfo m_prevBlockSelected;

	float m_blockBreakAge;
	bool m_currentlyBreakingABlock;
};