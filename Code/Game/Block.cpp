#include "Game/Block.hpp"

//STRUCTORS------------------------------------------------------------------------------------------------------------
Block::Block() 
    : m_blockType(0)
	, m_lightAndFlags(0)
{}
Block::Block(BlockType type) 
    : m_blockType(0)
	, m_lightAndFlags(0)
{
	m_blockType = (unsigned char)type;
}

//BLOCK TYPE------------------------------------------------------------------------------------------------------------
void Block::SetBlockType(BlockType type) {
	m_blockType = (unsigned char)type;
}
BlockType Block::GetBlockType() {
	return static_cast<BlockType>(m_blockType);
}

//IS SKY FLAG------------------------------------------------------------------------------------------------------------
void Block::SetIsSky() {
	SetBit(m_lightAndFlags, SKY_FLAG_MASK);
	SetLightLevel(9);
}
void Block::SetIsNotSky() {
	ClearBit(m_lightAndFlags, SKY_FLAG_MASK);
}
bool Block::GetIsSky() {
	return IsBitSet(m_lightAndFlags, SKY_FLAG_MASK);
}

//SKY DIRTY FLAG------------------------------------------------------------------------------------------------------------
void Block::SetIsDirty() {
	SetBit(m_lightAndFlags, SKY_ISDIRTY_FLAG_MASK);
}
void Block::SetIsNotDirty() {
	ClearBit(m_lightAndFlags, SKY_ISDIRTY_FLAG_MASK);
}
bool Block::IsDirty() {
	return IsBitSet(m_lightAndFlags, SKY_ISDIRTY_FLAG_MASK);
}

//IS OPAQUE FLAG------------------------------------------------------------------------------------------------------------
void Block::SetIsOpaque() {
	SetBit(m_lightAndFlags, OPAQUE_FLAG_MASK);

}
void Block::SetIsNotOpaque() {
	ClearBit(m_lightAndFlags, OPAQUE_FLAG_MASK);
}
bool Block::IsOpaque() {
	return IsBitSet(m_lightAndFlags, OPAQUE_FLAG_MASK);
}

//LIGHT LEVEL------------------------------------------------------------------------------------------------------------
void Block::SetLightLevel(int lightLevel) {
	m_lightAndFlags &= BLOCK_FLAGS_MASK;
	m_lightAndFlags |= (BYTE)lightLevel;
}
BYTE Block::GetLightLevel() {
	return (m_lightAndFlags & LIGHT_LEVEL_MASK);
}
RGBA Block::BlockTint() {
	float val = (float)GetLightLevel() / (float)MAX_LIGHT_LEVEL;
	return RGBA(val, val, val, 1.f);
}