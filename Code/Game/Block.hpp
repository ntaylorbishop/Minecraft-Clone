#pragma once

#include <vector>
#include "Engine/Renderer/Vertex.hpp"
#include "Engine/Renderer/RGBA.hpp"
#include "Game/GameCommon.hpp"

using namespace std;

enum BlockType {
	BLOCK_AIR = 0,
	BLOCK_GRASS,
	BLOCK_DIRT,
	BLOCK_STONE,
	BLOCK_WATER,
	BLOCK_SAND,
	BLOCK_COBBLESTONE,
	BLOCK_GLOWSTONE,
	BLOCK_SNOW,
	BLOCK_ICE,
	BLOCK_SNOWTOP,
	BLOCK_NUM_TYPES
};
class Block {
public:
	//STRUCTORS
	Block();
	Block(BlockType type);

	//BLOCK TYPE
	void SetBlockType(BlockType type);
	BlockType GetBlockType();

	//IS SKY FLAG
	void SetIsSky();
	void SetIsNotSky();
	bool GetIsSky();

	//IS SKY DIRTY FLAG
	void SetIsDirty();
	void SetIsNotDirty();
	bool IsDirty();

	//IS OPAQUE FLAG
	void SetIsOpaque();
	void SetIsNotOpaque();
	bool IsOpaque();

	//LIGHT LEVEL
	void SetLightLevel(int lightLevel);
	BYTE GetLightLevel();
	RGBA BlockTint();

private:
	unsigned char m_lightAndFlags;
	unsigned char m_blockType;
};