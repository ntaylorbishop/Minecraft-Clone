#define STATIC

#include "Game/BlockDefinition.hpp"

STATIC BlockDefinition* BlockDefinition::m_blockDefinitions[BLOCK_NUM_TYPES];

STATIC void BlockDefinition::InitializeBlockDefinitions() {
	if (DEBUG_EnableTextures) {
		m_blockDefinitions[BLOCK_AIR]         = new BlockDefinition(false, false, 0,  IntVector2(0, 0), IntVector2(0, 0), IntVector2(0, 0), RGBA(0.f, 0.f, 0.f, 0.f), 0.f);
		m_blockDefinitions[BLOCK_GRASS]       = new BlockDefinition(true,  true,  0,  IntVector2(1, 0), IntVector2(3, 0), IntVector2(2, 0), RGBA(1.f, 1.f, 1.f, 1.f), 1.f);
		m_blockDefinitions[BLOCK_DIRT]        = new BlockDefinition(true,  true,  0,  IntVector2(3, 0), IntVector2(3, 0), IntVector2(3, 0), RGBA(1.f, 1.f, 1.f, 1.f), 1.f);
		m_blockDefinitions[BLOCK_STONE]       = new BlockDefinition(true,  true,  0,  IntVector2(4, 0), IntVector2(4, 0), IntVector2(4, 0), RGBA(1.f, 1.f, 1.f, 1.f), 2.f);
		m_blockDefinitions[BLOCK_WATER]       = new BlockDefinition(true,  true,  15, IntVector2(3, 1), IntVector2(3, 1), IntVector2(3, 1), RGBA(0.f, 0.f, 0.f, 1.f), 1.f);
		m_blockDefinitions[BLOCK_SAND]        = new BlockDefinition(true,  true,  0,  IntVector2(5, 0), IntVector2(5, 0), IntVector2(5, 0), RGBA(1.f, 1.f, 1.f, 1.f), 1.f);
		m_blockDefinitions[BLOCK_COBBLESTONE] = new BlockDefinition(true,  true,  0,  IntVector2(1, 1), IntVector2(1, 1), IntVector2(1, 1), RGBA(1.f, 1.f, 1.f, 1.f), 2.f);
		m_blockDefinitions[BLOCK_GLOWSTONE]   = new BlockDefinition(true,  true,  15, IntVector2(7, 0), IntVector2(7, 0), IntVector2(7, 0), RGBA(1.f, 1.f, 1.f, 1.f), 1.f);
		m_blockDefinitions[BLOCK_SNOW]        = new BlockDefinition(true,  true,  0,  IntVector2(6, 1), IntVector2(3, 0), IntVector2(5, 1), RGBA(1.f, 1.f, 1.f, 1.f), 1.f);
		m_blockDefinitions[BLOCK_ICE]         = new BlockDefinition(false, true,  15, IntVector2(7, 1), IntVector2(7, 1), IntVector2(7, 1), RGBA(1.f, 1.f, 1.f, 1.f), 1.f);
		m_blockDefinitions[BLOCK_SNOWTOP]     = new BlockDefinition(false, false , 0, IntVector2(5, 1), IntVector2(5, 1), IntVector2(5, 1), RGBA(1.f, 1.f, 1.f, 1.f), 0.1f);
	}

	InitializeBlockSounds();
}
STATIC void BlockDefinition::InitializeBlockSounds() {
	string footstepSoundsPath = "Data/Sounds/Footsteps/";
	string digSoundsPath = "Data/Sounds/Dig/";
	string placeSoundsPath = "Data/Sounds/Dig/";

	//GRASS
	m_blockDefinitions[BLOCK_GRASS]->SetFootstepSounds({
		footstepSoundsPath + "GrassSteps1.wav",
		footstepSoundsPath + "GrassSteps2.wav",
		footstepSoundsPath + "GrassSteps3.wav",
		footstepSoundsPath + "GrassSteps4.wav",
		footstepSoundsPath + "GrassSteps5.wav",
		footstepSoundsPath + "GrassSteps6.wav"
	});
	m_blockDefinitions[BLOCK_GRASS]->SetDigSounds({
		digSoundsPath + "GrassDig1.wav",
		digSoundsPath + "GrassDig2.wav"
	});
	m_blockDefinitions[BLOCK_GRASS]->SetPlaceSounds({
		placeSoundsPath + "GrassPlace1.wav",
		placeSoundsPath + "GrassPlace2.wav",
		placeSoundsPath + "GrassPlace3.wav",
		placeSoundsPath + "GrassPlace4.wav",
		placeSoundsPath + "GrassPlace5.wav",
		placeSoundsPath + "GrassPlace6.wav"
	});

	//DIRT
	m_blockDefinitions[BLOCK_DIRT]->SetFootstepSounds({
		footstepSoundsPath + "GravelSteps1.wav",
		footstepSoundsPath + "GravelSteps2.wav",
		footstepSoundsPath + "GravelSteps3.wav",
		footstepSoundsPath + "GravelSteps4.wav",
		footstepSoundsPath + "GravelSteps5.wav",
		footstepSoundsPath + "GravelSteps6.wav"
	});
	m_blockDefinitions[BLOCK_DIRT]->SetDigSounds({
		digSoundsPath + "GravelDigPlace1.wav",
		digSoundsPath + "GravelDigPlace2.wav",
		digSoundsPath + "GravelDigPlace3.wav",
		digSoundsPath + "GravelDigPlace4.wav"
	});
	m_blockDefinitions[BLOCK_DIRT]->SetPlaceSounds({
		placeSoundsPath + "GravelDigPlace1.wav",
		placeSoundsPath + "GravelDigPlace2.wav",
		placeSoundsPath + "GravelDigPlace3.wav",
		placeSoundsPath + "GravelDigPlace4.wav"
	});

	//STONE
	m_blockDefinitions[BLOCK_STONE]->SetFootstepSounds({
		footstepSoundsPath + "StoneSteps1.wav",
		footstepSoundsPath + "StoneSteps2.wav",
		footstepSoundsPath + "StoneSteps3.wav",
		footstepSoundsPath + "StoneSteps4.wav",
		footstepSoundsPath + "StoneSteps5.wav",
		footstepSoundsPath + "StoneSteps6.wav"
	});
	m_blockDefinitions[BLOCK_STONE]->SetDigSounds({
		digSoundsPath + "StoneDigPlace1.wav",
		digSoundsPath + "StoneDigPlace2.wav",
		digSoundsPath + "StoneDigPlace3.wav",
		digSoundsPath + "StoneDigPlace4.wav"
	});
	m_blockDefinitions[BLOCK_STONE]->SetPlaceSounds({
		placeSoundsPath + "StoneDigPlace1.wav",
		placeSoundsPath + "StoneDigPlace2.wav",
		placeSoundsPath + "StoneDigPlace3.wav",
		placeSoundsPath + "StoneDigPlace4.wav"
	});

	//WATER
	m_blockDefinitions[BLOCK_WATER]->SetFootstepSounds({
		footstepSoundsPath + "WaterSteps1.wav",
		footstepSoundsPath + "WaterSteps2.wav",
		footstepSoundsPath + "WaterSteps3.wav",
		footstepSoundsPath + "WaterSteps4.wav"
	});
	m_blockDefinitions[BLOCK_WATER]->SetDigSounds({
		digSoundsPath + "WaterSteps1.wav"
	});
	m_blockDefinitions[BLOCK_WATER]->SetPlaceSounds({
		placeSoundsPath + "WaterSteps1.wav"
	});



	//SAND
	m_blockDefinitions[BLOCK_SAND]->SetFootstepSounds({
		footstepSoundsPath + "SandSteps1.wav",
		footstepSoundsPath + "SandSteps2.wav",
		footstepSoundsPath + "SandSteps3.wav",
		footstepSoundsPath + "SandSteps4.wav",
		footstepSoundsPath + "SandSteps5.wav"
	});
	m_blockDefinitions[BLOCK_SAND]->SetDigSounds({
		digSoundsPath + "SandDigPlace1.wav",
		digSoundsPath + "SandDigPlace2.wav",
		digSoundsPath + "SandDigPlace3.wav",
		digSoundsPath + "SandDigPlace4.wav"
	});
	m_blockDefinitions[BLOCK_SAND]->SetPlaceSounds({
		placeSoundsPath + "SandDigPlace1.wav",
		placeSoundsPath + "SandDigPlace2.wav",
		placeSoundsPath + "SandDigPlace3.wav",
		placeSoundsPath + "SandDigPlace4.wav"
	});

	//COBBLESTONE
	m_blockDefinitions[BLOCK_COBBLESTONE]->SetFootstepSounds({
		footstepSoundsPath + "StoneSteps1.wav",
		footstepSoundsPath + "StoneSteps2.wav",
		footstepSoundsPath + "StoneSteps3.wav",
		footstepSoundsPath + "StoneSteps4.wav",
		footstepSoundsPath + "StoneSteps5.wav",
		footstepSoundsPath + "StoneSteps6.wav"
	});
	m_blockDefinitions[BLOCK_COBBLESTONE]->SetDigSounds({
		digSoundsPath + "StoneDigPlace1.wav",
		digSoundsPath + "StoneDigPlace2.wav",
		digSoundsPath + "StoneDigPlace3.wav",
		digSoundsPath + "StoneDigPlace4.wav"
	});
	m_blockDefinitions[BLOCK_COBBLESTONE]->SetPlaceSounds({
		placeSoundsPath + "StoneDigPlace1.wav",
		placeSoundsPath + "StoneDigPlace2.wav",
		placeSoundsPath + "StoneDigPlace3.wav",
		placeSoundsPath + "StoneDigPlace4.wav"
	});

	//COBBLESTONE
	m_blockDefinitions[BLOCK_COBBLESTONE]->SetFootstepSounds({
		footstepSoundsPath + "StoneSteps1.wav",
		footstepSoundsPath + "StoneSteps2.wav",
		footstepSoundsPath + "StoneSteps3.wav",
		footstepSoundsPath + "StoneSteps4.wav",
		footstepSoundsPath + "StoneSteps5.wav",
		footstepSoundsPath + "StoneSteps6.wav"
	});
	m_blockDefinitions[BLOCK_COBBLESTONE]->SetDigSounds({
		digSoundsPath + "StoneDigPlace1.wav",
		digSoundsPath + "StoneDigPlace2.wav",
		digSoundsPath + "StoneDigPlace3.wav",
		digSoundsPath + "StoneDigPlace4.wav"
	});
	m_blockDefinitions[BLOCK_COBBLESTONE]->SetPlaceSounds({
		placeSoundsPath + "StoneDigPlace1.wav",
		placeSoundsPath + "StoneDigPlace2.wav",
		placeSoundsPath + "StoneDigPlace3.wav",
		placeSoundsPath + "StoneDigPlace4.wav"
	});

	//SNOW
	m_blockDefinitions[BLOCK_SNOW]->SetFootstepSounds({
		footstepSoundsPath + "SnowSteps1.wav",
		footstepSoundsPath + "SnowSteps2.wav",
		footstepSoundsPath + "SnowSteps3.wav",
		footstepSoundsPath + "SnowSteps4.wav"
	});
	m_blockDefinitions[BLOCK_SNOW]->SetDigSounds({
		digSoundsPath + "SnowDigPlace1.wav",
		digSoundsPath + "SnowDigPlace2.wav",
		digSoundsPath + "SnowDigPlace3.wav",
		digSoundsPath + "SnowDigPlace4.wav"
	});
	m_blockDefinitions[BLOCK_SNOW]->SetPlaceSounds({
		placeSoundsPath + "SnowDigPlace1.wav",
		placeSoundsPath + "SnowDigPlace2.wav",
		placeSoundsPath + "SnowDigPlace3.wav",
		placeSoundsPath + "SnowDigPlace4.wav"
	});

	//SNOWTOP
	m_blockDefinitions[BLOCK_SNOWTOP]->SetFootstepSounds({
		footstepSoundsPath + "SnowSteps1.wav",
		footstepSoundsPath + "SnowSteps2.wav",
		footstepSoundsPath + "SnowSteps3.wav",
		footstepSoundsPath + "SnowSteps4.wav"
	});
	m_blockDefinitions[BLOCK_SNOWTOP]->SetDigSounds({
		digSoundsPath + "SnowDigPlace1.wav",
		digSoundsPath + "SnowDigPlace2.wav",
		digSoundsPath + "SnowDigPlace3.wav",
		digSoundsPath + "SnowDigPlace4.wav"
	});
	m_blockDefinitions[BLOCK_SNOWTOP]->SetPlaceSounds({
		placeSoundsPath + "SnowDigPlace1.wav",
		placeSoundsPath + "SnowDigPlace2.wav",
		placeSoundsPath + "SnowDigPlace3.wav",
		placeSoundsPath + "SnowDigPlace4.wav"
	});

	//ICE
	m_blockDefinitions[BLOCK_ICE]->SetFootstepSounds({
		footstepSoundsPath + "SnowSteps1.wav",
		footstepSoundsPath + "SnowSteps2.wav",
		footstepSoundsPath + "SnowSteps3.wav",
		footstepSoundsPath + "SnowSteps4.wav"
	});
	m_blockDefinitions[BLOCK_ICE]->SetDigSounds({
		digSoundsPath + "SnowDigPlace1.wav",
		digSoundsPath + "SnowDigPlace2.wav",
		digSoundsPath + "SnowDigPlace3.wav",
		digSoundsPath + "SnowDigPlace4.wav"
	});
	m_blockDefinitions[BLOCK_ICE]->SetPlaceSounds({
		placeSoundsPath + "SnowDigPlace1.wav",
		placeSoundsPath + "SnowDigPlace2.wav",
		placeSoundsPath + "SnowDigPlace3.wav",
		placeSoundsPath + "SnowDigPlace4.wav"
	});


	//GLOWSTONE
	m_blockDefinitions[BLOCK_GLOWSTONE]->SetFootstepSounds({
		footstepSoundsPath + "WoodSteps1.wav",
		footstepSoundsPath + "WoodSteps2.wav",
		footstepSoundsPath + "WoodSteps3.wav",
		footstepSoundsPath + "WoodSteps4.wav",
		footstepSoundsPath + "WoodSteps5.wav",
		footstepSoundsPath + "WoodSteps6.wav"
	});
	m_blockDefinitions[BLOCK_GLOWSTONE]->SetDigSounds({
		digSoundsPath + "GlowstonePlace1.wav"
	});
	m_blockDefinitions[BLOCK_GLOWSTONE]->SetPlaceSounds({
		placeSoundsPath + "GlowstonePlace1.wav"
	});
}
STATIC void BlockDefinition::FlushBlockDefinitions() {
	delete m_blockDefinitions[BLOCK_AIR];
	delete m_blockDefinitions[BLOCK_GRASS];
	delete m_blockDefinitions[BLOCK_DIRT];
	delete m_blockDefinitions[BLOCK_STONE];
	delete m_blockDefinitions[BLOCK_WATER];
	delete m_blockDefinitions[BLOCK_SAND];
	delete m_blockDefinitions[BLOCK_COBBLESTONE];
	delete m_blockDefinitions[BLOCK_GLOWSTONE];
	delete m_blockDefinitions[BLOCK_SNOW];
	delete m_blockDefinitions[BLOCK_ICE];
}

STATIC BlockDefinition* BlockDefinition::GetBlockDefinitionByType(BlockType type) {
	return m_blockDefinitions[type];
}

//ACCESSORS MUTATORS---------------------------------------------------------------------------------------------------------------------------------
STATIC bool BlockDefinition::IsOpaque(BlockType type) {
	return m_blockDefinitions[type]->IsOpaque();
}
STATIC bool BlockDefinition::IsSolid(BlockType type) {
	return m_blockDefinitions[type]->IsSolid();
}
STATIC IntVector2 BlockDefinition::SpriteSides(BlockType type) {
	return m_blockDefinitions[type]->SpriteSides();
}
STATIC IntVector2 BlockDefinition::SpriteBottom(BlockType type) {
	return m_blockDefinitions[type]->SpriteBottom();
}
STATIC IntVector2 BlockDefinition::SpriteTop(BlockType type) {
	return m_blockDefinitions[type]->SpriteTop();
}
STATIC RGBA BlockDefinition::BlockTint(BlockType type) {
	return m_blockDefinitions[type]->BlockTint();
}
STATIC int BlockDefinition::InternalLightValue(BlockType type) {
	return m_blockDefinitions[type]->InternalLightValue();
}
STATIC float BlockDefinition::GetBlockToughness(BlockType type) {
	return m_blockDefinitions[type]->GetBlockToughness();
}
STATIC vector<string> BlockDefinition::GetBlockFootstepSounds(BlockType type) {
	return m_blockDefinitions[type]->GetBlockFootstepSounds();
}
STATIC vector<string> BlockDefinition::GetBlockDigSounds(BlockType type) {
	return m_blockDefinitions[type]->GetBlockDigSounds();
}
STATIC vector<string> BlockDefinition::GetBlockPlaceSounds(BlockType type) {
	return m_blockDefinitions[type]->GetBlockPlaceSounds();
}


//STRUCTORS---------------------------------------------------------------------------------------------------------------------------------
BlockDefinition::BlockDefinition()
    : m_isOpaque(false)
    , m_isSolid(false)
    , m_internalLightValue(-1)
    , m_spriteSides()
    , m_spriteBottom()
    , m_spriteTop()
	, m_blockTint()
	, m_blockToughness(0.f)
{ }
BlockDefinition::BlockDefinition(bool isOpaque, bool isSolid, int internalLightValue, const IntVector2& spriteSides, const IntVector2& spriteBottom, const IntVector2& spriteTop, const RGBA& blockTint, float blockToughness)
    : m_isOpaque(isOpaque)
    , m_isSolid(isSolid)
    , m_internalLightValue(internalLightValue)
    , m_spriteSides(spriteSides)
    , m_spriteBottom(spriteBottom)
    , m_spriteTop(spriteTop)
	, m_blockTint(blockTint)
	, m_blockToughness(blockToughness)
{ }
BlockDefinition::~BlockDefinition() { }

//PRIVATE ACCESSORS---------------------------------------------------------------------------------------------------------------------------------
bool BlockDefinition::IsOpaque() {
	return m_isOpaque;
}
bool BlockDefinition::IsSolid() {
	return m_isSolid;
}
IntVector2 BlockDefinition::SpriteSides() {
	return m_spriteSides;
}
IntVector2 BlockDefinition::SpriteBottom() {
	return m_spriteBottom;
}
IntVector2 BlockDefinition::SpriteTop() {
	return m_spriteTop;
}
RGBA BlockDefinition::BlockTint() {
	return m_blockTint;
}
int BlockDefinition::InternalLightValue() {
	return m_internalLightValue;
}
float BlockDefinition::GetBlockToughness() {
	return m_blockToughness;
}
void BlockDefinition::SetDigSounds(const vector<string>& digSounds) {
	m_digSounds = digSounds;
}
void BlockDefinition::SetPlaceSounds(const vector<string>& placeSounds) {
	m_placeSounds = placeSounds;
}
void BlockDefinition::SetFootstepSounds(const vector<string>& footstepSounds) {
	m_footstepSounds = footstepSounds;
}
vector<string> BlockDefinition::GetBlockFootstepSounds() {
	return m_footstepSounds;
}
vector<string> BlockDefinition::GetBlockDigSounds() {
	return m_digSounds;
}
vector<string> BlockDefinition::GetBlockPlaceSounds() {
	return m_placeSounds;
}