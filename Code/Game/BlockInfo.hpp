#pragma once

#include "Game/Block.hpp"
#include "Game/Chunk.hpp"

class BlockInfo {
public:
	BlockInfo() 
		: chunk(nullptr)
		, m_idx(-1) 
	{}
	BlockInfo(Chunk* chunkPtr, int blockIdx) 
		: chunk(chunkPtr)
		, m_idx(blockIdx) 
	{}

	Chunk* chunk;
	BlockIndex m_idx;

	Block* GetBlock();

	BlockInfo GetBlockToNorth() const;
	BlockInfo GetBlockToEast() const;
	BlockInfo GetBlockToSouth() const;
	BlockInfo GetBlockToWest() const;
	BlockInfo GetBlockAbove() const;
	BlockInfo GetBlockBelow() const;

	WorldPosition GetBlockWorldPosition() const;

	bool operator==(const BlockInfo& other) const {
		if (m_idx == other.m_idx) {
			if (chunk->GetChunkPosition() == other.chunk->GetChunkPosition())
				return true;
			else
				return false;
		}
		else
			return false;
	}
	bool operator!=(const BlockInfo& other) const {
		return !(*this == other);
	}
};

inline Block* BlockInfo::GetBlock() {
	if (m_idx >= 0 && m_idx < BLOCKS_PER_CHUNK)
		return &chunk->m_blocks[m_idx];
	else
		return nullptr;
}