#pragma once
#define STATIC //Do-nothing to indicate static

#include <vector>
#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Spritesheet.hpp"
#include "Engine/Renderer/Vertex.hpp"
#include "Engine/Math/VectorUtils.hpp"
#include "Engine/Math/PerlinNoise.hpp"
#include "Engine/Time.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Block.hpp"
#include "Game/BlockDefinition.hpp"
#include "Game/WeatherSheet.hpp"
#include "Game/TheCloudSystem.hpp"

using namespace std;

enum BlockFace {
	FACE_TOP = 0,
	FACE_BOTTOM,
	FACE_NORTH,
	FACE_EAST,
	FACE_SOUTH,
	FACE_WEST,
	FACE_NONE
};

struct BlockSelected {
	Block* block;
	BlockFace face;
};

const int SHEETS_PER_COLUMN = 20;

class Chunk {
public:
	//STRUCTORS
	Chunk(const ChunkCoords& chunkPosition);
	Chunk(const ChunkCoords& chunkPosition, const vector<unsigned char>& buffer);
	~Chunk();

	//CHUNK GENERATION
	void InitializeGeneratedChunk();
	void GenerateVBOID();
	void GenerateFromPerlinNoise();
	void GenerateFromFile(const vector<unsigned char>& buffer);
	void InitializeLightingColumnHeightsAndSnowTops();

	void InitializeChunkBiomesAndHeights();
	void InitializeWeather();

	//UPDATE
	bool Update();
	void UpdateWeather(float deltaSeconds);
	void UpdateSnow();

	void PlaceBlock(const WorldPosition& blockPos, BlockType type);
	void DeleteBlock(const WorldPosition& blockPos);

	//RENDER
	void RenderWeather() const;

	void MarkVertexArrayDirty(bool isDirty);

	bool IsChunkVisible(const WorldPosition& cameraDirection, const WorldPosition& cameraPos);

	int GetVBOID();
	int GetNumVertexes();
	
	BIOME GetColumnBiome(const WorldPosition& worldPos);

	//COORDINATE SWITCHING
	ChunkCoords GetChunkPosition() const;
	WorldPosition GetWorldPositionOfBlock(BlockIndex idx);
	WorldPosition GetChunkWorldPosition();
	WorldPosition GetTopOfColumnInWorldPosition(const ColumnCoords& column);
	LocalCoords GetTopOfColumnInLocalCoords(const ColumnCoords& column);
	bool IsBlockPositionOnNorthBorderOfChunk(const LocalCoords& blockLoc);
	bool IsBlockPositionOnEastBorderOfChunk(const LocalCoords& blockLoc);
	bool IsBlockPositionOnSouthBorderOfChunk(const LocalCoords& blockLoc);
	bool IsBlockPositionOnWestBorderOfChunk(const LocalCoords& blockLoc);

	static inline LocalCoords GetLocalCoordsForBlockIndex(int idx);
	static inline int GetBlockIndexForLocalCoords(const LocalCoords& position);
	static inline ColumnCoords GetColumnCoordsFromIndex(int idx);
	static inline LocalCoords GetLocalCoordsForWorldCoords(const WorldPosition& position);
	static inline WorldPosition GetWorldCoordsForLocalCoords(const WorldPosition& position);
	static inline ChunkCoords GetChunkCoordsForWorldCoords(const WorldPosition& worldPos);
	static inline LocalCoords GetLocalCoordsForLocalCoordsOutOfBounds(LocalCoords loc);
	static inline WorldPosition GetWorldPositionOfBlockInChunk(const Chunk* chunk, const LocalCoords& localPos);
	static inline bool IsBlockPositionWithinBounds(const LocalCoords& pos);
	static inline WorldPosition GetWorldPositionForChunkCoords(const ChunkCoords& loc);

public:
	Block m_blocks[BLOCKS_PER_CHUNK];
	Chunk* m_northNeighbor;
	Chunk* m_southNeighbor;
	Chunk* m_eastNeighbor;
	Chunk* m_westNeighbor;

private:
	SpriteSheet* m_spriteSheet;
	ChunkCoords m_chunkPosition;
	bool m_needsToBeChanged;
	BlockSelected m_selectedBlock;

	int m_vboID;
	int m_numVertexes;

	//Biomes
	BIOME m_columnBiome[BLOCKS_WIDE_X * BLOCKS_WIDE_Y];
	float m_columnHeight[BLOCKS_WIDE_X * BLOCKS_WIDE_Y];

	//Weather
	vector<WeatherSheet> m_snowSheets;
	vector<WeatherSheet> m_rainSheets;

	//PRIVATE UTILITY
	void ComputeChunkVertices();
	bool IsFaceShowing(int idx, BlockFace face);
	void PushVertices(const Vector3& bottomLeft, const Vector3& bottomRight, const Vector3& topRight, const Vector3& topLeft, const SpriteCoords& spriteCoords, const RGBA& tint, vector<TexturedVertex>& vertices);
};









//INLINE COORDINATE SWITCHING OPERATIONS-------------------------------------------------------------------------------------------
inline ChunkCoords Chunk::GetChunkPosition() const {
	return m_chunkPosition;
}
inline WorldPosition Chunk::GetWorldPositionOfBlock(BlockIndex idx) {
	LocalCoords blockCoords = GetLocalCoordsForBlockIndex(idx);
	return WorldPosition((float)m_chunkPosition.x * (float)BLOCKS_WIDE_X + (float)blockCoords.x, (float)m_chunkPosition.y * (float)BLOCKS_WIDE_Y + (float)blockCoords.y, (float)blockCoords.z);
}
inline WorldPosition Chunk::GetChunkWorldPosition() {
	return WorldPosition((float)m_chunkPosition.x * (float)BLOCKS_WIDE_X, (float)m_chunkPosition.y * (float)BLOCKS_WIDE_Y, 0.f);
}
inline WorldPosition Chunk::GetTopOfColumnInWorldPosition(const ColumnCoords& column) {
	return WorldPosition((float)m_chunkPosition.x * (float)BLOCKS_WIDE_X, (float)m_chunkPosition.y * (float)BLOCKS_WIDE_Y, 0.f) + WorldPosition((float)column.x, (float)column.y, (float)(BLOCKS_WIDE_Z - 1));
}
inline LocalCoords Chunk::GetTopOfColumnInLocalCoords(const ColumnCoords& column) {
	return LocalCoords(column.x, column.y, BLOCKS_WIDE_Z - 1);
}


STATIC LocalCoords Chunk::GetLocalCoordsForBlockIndex(int idx) {
	return LocalCoords(idx & LOCAL_X_MASK, (idx >> CHUNK_BITS_X) & LOCAL_Y_MASK, idx >> BITS_XY);
}
STATIC int Chunk::GetBlockIndexForLocalCoords(const LocalCoords& position) {
	return position.x | (position.y << CHUNK_BITS_X) | (position.z << BITS_XY);
}
STATIC ColumnCoords Chunk::GetColumnCoordsFromIndex(int idx) {
	return ColumnCoords(idx & LOCAL_X_MASK, (idx >> CHUNK_BITS_X) & LOCAL_Y_MASK);
}
STATIC LocalCoords Chunk::GetLocalCoordsForWorldCoords(const WorldPosition& position) {
	int x = (int)floor(position.x);
	int y = (int)floor(position.y);
	int z = (int)floor(position.z);

	return LocalCoords((x & LOCAL_X_MASK), (y & LOCAL_Y_MASK), z);
}
STATIC WorldPosition Chunk::GetWorldCoordsForLocalCoords(const WorldPosition& position) {
	float x = (float)floor(position.x);
	float y = (float)floor(position.y);
	float z = (float)floor(position.z);

	return WorldPosition(x, y, z);
}
STATIC ChunkCoords Chunk::GetChunkCoordsForWorldCoords(const WorldPosition& worldPos) {
	int x = (int)floor(worldPos.x);
	int y = (int)floor(worldPos.y);

	return ChunkCoords((x >> CHUNK_BITS_X), (y >> CHUNK_BITS_Y));
}
STATIC LocalCoords Chunk::GetLocalCoordsForLocalCoordsOutOfBounds(LocalCoords loc) {
	return LocalCoords((loc.x & LOCAL_X_MASK), (loc.y & LOCAL_Y_MASK), loc.z);
}
STATIC WorldPosition Chunk::GetWorldPositionOfBlockInChunk(const Chunk* chunk, const LocalCoords& localPos) {
	ChunkCoords chunkLoc = chunk->GetChunkPosition();
	return WorldPosition((float)(chunkLoc.x * BLOCKS_WIDE_X + localPos.x), (float)(chunkLoc.y * BLOCKS_WIDE_Y + localPos.y), (float)localPos.z);
}
STATIC bool Chunk::IsBlockPositionWithinBounds(const LocalCoords& pos) {
	return (pos.x < BLOCKS_WIDE_X && pos.x >= 0.f && pos.y < BLOCKS_WIDE_Y && pos.y >= 0.f && pos.z < BLOCKS_WIDE_Z && pos.z >= 0.f);
}
STATIC WorldPosition Chunk::GetWorldPositionForChunkCoords(const ChunkCoords& loc) {
	return WorldPosition((float)loc.x * (float)BLOCKS_WIDE_X, (float)loc.y * (float)BLOCKS_WIDE_Y, 0.f);
}

inline bool Chunk::IsBlockPositionOnNorthBorderOfChunk(const LocalCoords& blockLoc) {
	return (blockLoc.x == 0);
}
inline bool Chunk::IsBlockPositionOnEastBorderOfChunk(const LocalCoords& blockLoc) {
	return (blockLoc.y == BLOCKS_WIDE_Y - 1);
}
inline bool Chunk::IsBlockPositionOnSouthBorderOfChunk(const LocalCoords& blockLoc) {
	return (blockLoc.x == BLOCKS_WIDE_X - 1);
}
inline bool Chunk::IsBlockPositionOnWestBorderOfChunk(const LocalCoords& blockLoc) {
	return (blockLoc.y == 0);
}