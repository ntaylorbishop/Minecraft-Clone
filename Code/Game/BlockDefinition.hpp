#pragma once

#include <string>
#include "Engine/Renderer/RGBA.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Block.hpp"

using namespace std;
class BlockDefinition {
public:
	static void BlockDefinition::InitializeBlockDefinitions();
	static void BlockDefinition::InitializeBlockSounds();
	static void BlockDefinition::FlushBlockDefinitions();

	static BlockDefinition* BlockDefinition::GetBlockDefinitionByType(BlockType type);

	//ACCESSORS MUTATORS
	static bool IsOpaque(BlockType type);
	static bool IsSolid(BlockType type);
	static IntVector2 SpriteSides(BlockType type);
	static IntVector2 SpriteBottom(BlockType type);
	static IntVector2 SpriteTop(BlockType type);
	static RGBA BlockTint(BlockType type);
	static int InternalLightValue(BlockType type);
	static float GetBlockToughness(BlockType type);
	static vector<string> GetBlockFootstepSounds(BlockType type);
	static vector<string> GetBlockDigSounds(BlockType type);
	static vector<string> GetBlockPlaceSounds(BlockType type);

private:
	//STRUCTORS
	BlockDefinition();
	BlockDefinition(bool isOpaque, bool isSolid, int internalLightValue, const IntVector2& spriteSides, const IntVector2& spriteBottom, const IntVector2& spriteTop, const RGBA& blockTint, float blockToughness);
	~BlockDefinition();

	bool IsOpaque();
	bool IsSolid();
	IntVector2 SpriteSides();
	IntVector2 SpriteBottom();
	IntVector2 SpriteTop();
	RGBA BlockTint();
	int InternalLightValue();
	float GetBlockToughness();
	void SetDigSounds(const vector<string>& digSounds);
	void SetPlaceSounds(const vector<string>& placeSounds);
	void SetFootstepSounds(const vector<string>& footstepSounds);
	vector<string> GetBlockFootstepSounds();
	vector<string> GetBlockDigSounds();
	vector<string> GetBlockPlaceSounds();


	bool m_isOpaque;
	bool m_isSolid;
	int m_internalLightValue;
	float m_blockToughness;

	IntVector2 m_spriteSides;
	IntVector2 m_spriteBottom;
	IntVector2 m_spriteTop;

	RGBA m_blockTint;

	vector<string> m_digSounds;
	vector<string> m_placeSounds;
	vector<string> m_footstepSounds;

	static BlockDefinition* m_blockDefinitions[BLOCK_NUM_TYPES];
};