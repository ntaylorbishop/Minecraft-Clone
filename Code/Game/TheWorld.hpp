#pragma once

#include <deque>
#include <map>
#include <set>
#include <utility>
#include <stdio.h>
#include <thread>
#include "Engine/InputSystem.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/FileUtils.hpp"
#include "Engine/Math/PolarCoords.hpp"
#include "Engine/Time.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Chunk.hpp"
#include "Game/BlockInfo.hpp"
#include "Game/TheCloudSystem.hpp"


class TheWorld;
extern TheWorld* g_theWorld;

using namespace std;

class TheWorld {
public:
	//STRUCTORS
	TheWorld();
	~TheWorld();

	//INITIALIZATION
	void InitializeThreads();
	void InitializeWorld();

	//THREADED CHUNK GENERATION
	static void ChunkGenerationThreadMain();

	//UPDATE CYCLE
	void Update(const WorldPosition& playerPos, float deltaSeconds);

	void UpdateWeather(float deltaSeconds);
	void UpdateSkyForWeather();
	void ModulateSkyValueAndPlayRainSound();

	void UpdateLighting();
	bool UpdateAVertexArrayIfDirty();

	//LIGHTING
	void UpdateLightingForBlock(BlockInfo bi);
	int CalcIdealLightForBlock(BlockInfo bi);
	void MarkAsLightingDirty(BlockInfo bi);
	void SetBlockNeighborsDirty(BlockInfo bi);

	//RENDER
	void Render(const WorldPosition& cameraDirection, const WorldPosition& cameraPos) const;

	//CHUNK GENERATION
	void AddGeneratedChunks();
	bool RegisterAndConnectOneChunk(const ChunkCoords& playerCenter);
	bool DeregisterAndDeconnectOneChunk(const ChunkCoords& playerCenter);
	void UpdateNeighbors(const ChunkCoords& chunkLoc);

	//COORDINATE SWITCHING
	Block* GetBlockForWorldCoords(const WorldPosition& position);
	static BlockInfo GetBlockInfoForWorldPosition(const WorldPosition& worldPos);
	static BlockInfo GetBlockInfoForLocalCoords(const LocalCoords& loc);

public:
	map<ChunkCoords, Chunk*> m_activeChunks;
	static deque<ChunkCoords> m_chunkRequests; //THIS IS THE CRITICAL SECTION BETWEEN THREADS
	static set<Chunk*> m_finishedGenChunks;

private:
	SpriteSheet* m_spriteSheet;

	//LIGHTING
	deque<BlockInfo> m_dirtyBlocks;

	//WEATHER
	float m_closestWeatherSheet;
};