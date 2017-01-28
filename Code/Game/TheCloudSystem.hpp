#pragma once

#include <map>
#include "Engine/Math/Vector3.hpp"
#include "Engine/Renderer/TheRenderer.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Cloud.hpp"

const float CLOUD_MIN_SPEED = 1.f;
const float CLOUD_MAX_SPEED = 4.f;
const float CLOUD_X_MIN_SIZE = 2.f;
const float CLOUD_X_MAX_SIZE = 15.f;
const float CLOUD_Y_MIN_SIZE = 2.f;
const float CLOUD_Y_MAX_SIZE = 15.f;
const float CLOUD_Z_MIN_SIZE = 1.f;
const float CLOUD_Z_MAX_SIZE = 1.f;
const float CLOUD_HEIGHT = 128.f;

using namespace std;

class Chunk;
class TheCloudSystem;

extern TheCloudSystem* g_theCloudSystem;

class TheCloudSystem {
public:
	TheCloudSystem();
	
	void InitializeCloudsForChunk(Chunk* chunk);
	void FlushCloudsForChunk(Chunk* chunk);

	void Update(float deltaSeconds);

	void Render() const;

private:
	void ComputeCloudVertices(Cloud* cloud);
	float GetNumberForRandomSeed(int randomNumber, int whichAxis);

private:
	map<ChunkCoords, Cloud*> m_clouds;
	vector<Vertex> m_vertices;
};