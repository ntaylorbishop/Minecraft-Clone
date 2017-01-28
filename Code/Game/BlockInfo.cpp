#include "Game/BlockInfo.hpp"

BlockInfo BlockInfo::GetBlockToNorth() const {
	if (chunk != nullptr) {
		LocalCoords blockLoc = Chunk::GetLocalCoordsForBlockIndex(m_idx) + LocalCoords(-1, 0, 0);
		if (Chunk::IsBlockPositionWithinBounds(blockLoc)) {
			BlockIndex idx = Chunk::GetBlockIndexForLocalCoords(blockLoc);

			return BlockInfo(chunk, idx);
		}
		else if (chunk->m_northNeighbor != nullptr) {
			blockLoc = Chunk::GetLocalCoordsForLocalCoordsOutOfBounds(blockLoc);
			BlockIndex idx = Chunk::GetBlockIndexForLocalCoords(blockLoc);

			return BlockInfo(chunk->m_northNeighbor, idx);
		}
		else
			return BlockInfo();
	}
	else
		return BlockInfo();
}
BlockInfo BlockInfo::GetBlockToEast() const {
	if (chunk != nullptr) {
		LocalCoords blockLoc = Chunk::GetLocalCoordsForBlockIndex(m_idx) + LocalCoords(0, 1, 0);
		if (Chunk::IsBlockPositionWithinBounds(blockLoc)) {
			BlockIndex idx = Chunk::GetBlockIndexForLocalCoords(blockLoc);

			return BlockInfo(chunk, idx);
		}
		else if (chunk->m_eastNeighbor != nullptr) {
			blockLoc = Chunk::GetLocalCoordsForLocalCoordsOutOfBounds(blockLoc);
			BlockIndex idx = Chunk::GetBlockIndexForLocalCoords(blockLoc);

			return BlockInfo(chunk->m_eastNeighbor, idx);
		}
		else
			return BlockInfo();
	}
	else
		return BlockInfo();
}
BlockInfo BlockInfo::GetBlockToSouth() const {
	if (chunk != nullptr) {
		LocalCoords blockLoc = Chunk::GetLocalCoordsForBlockIndex(m_idx) + LocalCoords(1, 0, 0);
		if (Chunk::IsBlockPositionWithinBounds(blockLoc)) {
			BlockIndex idx = Chunk::GetBlockIndexForLocalCoords(blockLoc);

			return BlockInfo(chunk, idx);
		}
		else if (chunk->m_southNeighbor != nullptr) {
			blockLoc = Chunk::GetLocalCoordsForLocalCoordsOutOfBounds(blockLoc);
			BlockIndex idx = Chunk::GetBlockIndexForLocalCoords(blockLoc);

			return BlockInfo(chunk->m_southNeighbor, idx);
		}
		else
			return BlockInfo();
	}
	else
		return BlockInfo();
}
BlockInfo BlockInfo::GetBlockToWest() const {
	if (chunk != nullptr) {
		LocalCoords blockLoc = Chunk::GetLocalCoordsForBlockIndex(m_idx) + LocalCoords(0, -1, 0);
		if (Chunk::IsBlockPositionWithinBounds(blockLoc)) {
			BlockIndex idx = Chunk::GetBlockIndexForLocalCoords(blockLoc);

			return BlockInfo(chunk, idx);
		}
		else if (chunk->m_westNeighbor != nullptr) {
			blockLoc = Chunk::GetLocalCoordsForLocalCoordsOutOfBounds(blockLoc);
			BlockIndex idx = Chunk::GetBlockIndexForLocalCoords(blockLoc);

			return BlockInfo(chunk->m_westNeighbor, idx);
		}
		else
			return BlockInfo();
	}
	else
		return BlockInfo();
}

BlockInfo BlockInfo::GetBlockAbove() const {
	if (chunk != nullptr) {
		LocalCoords blockLoc = Chunk::GetLocalCoordsForBlockIndex(m_idx) + LocalCoords(0, 0, 1);
		if (Chunk::IsBlockPositionWithinBounds(blockLoc)) {
			BlockIndex idx = Chunk::GetBlockIndexForLocalCoords(blockLoc);

			return BlockInfo(chunk, idx);
		}
		else
			return BlockInfo();
	}
	else
		return BlockInfo();
}
BlockInfo BlockInfo::GetBlockBelow() const {
	if (chunk != nullptr) {
		LocalCoords blockLoc = Chunk::GetLocalCoordsForBlockIndex(m_idx) + LocalCoords(0, 0, -1);
		if (Chunk::IsBlockPositionWithinBounds(blockLoc)) {
			BlockIndex idx = Chunk::GetBlockIndexForLocalCoords(blockLoc);

			return BlockInfo(chunk, idx);
		}
		else
			return BlockInfo();
	}
	else
		return BlockInfo();
}

WorldPosition BlockInfo::GetBlockWorldPosition() const {
	return Chunk::GetWorldPositionOfBlockInChunk(chunk, Chunk::GetLocalCoordsForBlockIndex(m_idx));
}