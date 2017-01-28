#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <vector>
#include <string>
#include "Engine/EngineCommon.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/IntVector3.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Renderer/RGBA.hpp"

#define BIT(x) (1 << (x))

typedef Vector3 WorldPosition;
typedef IntVector3 LocalCoords;
typedef IntVector3 GlobalCoords;
typedef IntVector2 ChunkCoords;
typedef IntVector2 ColumnCoords;
typedef int BlockIndex;


using namespace std;

//PLAYER---------------------------------------------------------------------------------------------------------------------------------------------
const float MOUSE_SENSITIVITY = 0.08f;

const float PLAYER_MOVE_SPEED_NOCLIP = 8.f;
const float PLAYER_MOVE_SPEED_WALKING = 4.f;
const float PLAYER_SHIFT_MOVE_SPEED = 5.f;

const float PLAYER_FRICTION_COEFFICIENT = 0.95f; //Always between 0.0 ... 1.0
const float PLAYER_MAX_VELOCITY_NOCLIP = 20.f;
const float PLAYER_MAX_VELOCITY_WALKING = 8.f; //= 10.f;
const float PLAYER_GRAVITY = 0.5f;
const float PLAYER_JUMP_SPEED = 20.f;

const int PLAYER_SIGHT_RADIUS = 50;
const int CHUNK_GENERATION_OUTER_RADIUS = PLAYER_SIGHT_RADIUS + 1;
const int CHUNK_GENERATION_INNER_RADIUS = PLAYER_SIGHT_RADIUS;

const float PLAYER_SELECTION_RANGE = 9.f;

//UTILITY---------------------------------------------------------------------------------------------------------------------------------------------
const int NUM_SAMPLE_POINTS = 5;
const float ANGLE_STEP = 360.f / (float)NUM_SAMPLE_POINTS;

//CHUNKS---------------------------------------------------------------------------------------------------------------------------------------------
const int BLOCK_SIZE = 1;

const int BASE_HEIGHT = 80;
const int CRUST_BOTTOM = BASE_HEIGHT - 10;
const int CRUST_TOP = BASE_HEIGHT + 9;
const int GRASS_TOP = BASE_HEIGHT + 9;
const int SEA_LEVEL = BASE_HEIGHT - 11;

//BIT SHIFTING STUFF---------------------------------------------------------------------------------------------------------------------------------------------
const BYTE CHUNK_BITS_X = 4;
const BYTE CHUNK_BITS_Y = 4;
const BYTE CHUNK_BITS_Z = 7;

const BYTE BLOCKS_WIDE_X = BIT(CHUNK_BITS_X);
const BYTE BLOCKS_WIDE_Y = BIT(CHUNK_BITS_Y);
const BYTE BLOCKS_WIDE_Z = BIT(CHUNK_BITS_Z);

const BYTE LOCAL_X_MASK = BLOCKS_WIDE_X - 1;
const BYTE LOCAL_Y_MASK = BLOCKS_WIDE_Y - 1;
const BYTE LOCAL_Z_MASK = BLOCKS_WIDE_Z - 1;
const BYTE BITS_XY = CHUNK_BITS_X + CHUNK_BITS_Y;

const int BLOCKS_PER_LAYER = BLOCKS_WIDE_X * BLOCKS_WIDE_Y;
const int BLOCKS_PER_CHUNK = BLOCKS_WIDE_X * BLOCKS_WIDE_Y * BLOCKS_WIDE_Z;

const BYTE LIGHTING_BITS = 4;
const BYTE MAX_LIGHT_LEVEL = ((1 << LIGHTING_BITS) - 1);
const BYTE LIGHTING_MASK = MAX_LIGHT_LEVEL;
const BYTE SKY_LIGHT_LEVEL = 9;

const BYTE SKY_FLAG_MASK = 128;
const BYTE SKY_ISDIRTY_FLAG_MASK = 64;
const BYTE OPAQUE_FLAG_MASK = 32;
const BYTE LIGHT_LEVEL_MASK = 15;
const BYTE BLOCK_FLAGS_MASK = 240;


extern const char* SPRITESHEET_TO_USE;

//BIOME CONSTANTS---------------------------------------------------------------------------------------------------------------------------------------------
enum BIOME {
	BIOME_MOUNTAIN = 0,
	BIOME_DESERT,
	BIOME_PLAINS,
	BIOME_TUNDRA
};

std::string GetBiomeStringified(BIOME biome);

//WEATHER--------------------------------------------------------------------------------------------------------------------------------------------
extern RGBA g_skyColor;
const RGBA SKY_COLOR_RAIN = RGBA(0.f, 0.20f, 0.34117f, 1.f);
const RGBA SKY_COLOR_REGULAR = RGBA(0x00, 0x59, 0x87, 0xff);

//FILE PATHS
extern const char* FOOTSTEP_SOUNDS_PATH;
extern const char* DIG_SOUNDS_PATH;

//DEBUG TOGGLERS---------------------------------------------------------------------------------------------------------------------------------------------
extern const bool DEBUG_RenderJustOneChunk;
extern const bool DEBUG_EnableDebugDrawing;
extern const bool DEBUG_EnableFileSaveLoad;
extern const bool DEBUG_EnableTextures;
extern const bool DEBUG_EnableLighting;
extern const bool DEBUG_FrameMustBeEnabled;

//BIT OPERATIONS------------------------------------------------------------------------------------------------------------------------------------------
inline void SetBit(unsigned char& bitFlags, unsigned char bitMask) {
	bitFlags |= bitMask;
	bitFlags = bitFlags | bitMask;
}
inline bool IsBitSet(unsigned char bitFlags, unsigned char bitMask) {
	return (bitFlags & bitMask) != 0;
}
inline void ClearBit(unsigned char& bitFlags, unsigned char bitMask) {
	bitFlags &= ~bitMask;
}

//MULTITHREADING------------------------------------------------------------------------------------------------------------------------------------------
extern CRITICAL_SECTION g_chunkListsCS;

//RUNNING------------------------------------------------------------------------------------------------------------------------------------------
extern bool g_isRunning;