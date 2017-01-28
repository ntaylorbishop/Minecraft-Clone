#include "Game/GameCommon.hpp"

const char* SPRITESHEET_TO_USE = "Data/Images/SimpleMinerAtlas.png";

//DEBUG TOGGLERS------------------------------------------------------------------------------------------------------------------------------------------
const bool DEBUG_RenderJustOneChunk = false;
const bool DEBUG_EnableDebugDrawing = true;
const bool DEBUG_EnableFileSaveLoad = true;
const bool DEBUG_EnableTextures = true;
const bool DEBUG_EnableLighting = true;
const bool DEBUG_FrameMustBeEnabled = false;

//BIOMES------------------------------------------------------------------------------------------------------------------------------------------
std::string GetBiomeStringified(BIOME biome) {
	if (biome == BIOME_MOUNTAIN)
		return "Mountains";
	else if (biome == BIOME_DESERT)
		return "Deserts";
	else if (biome == BIOME_PLAINS)
		return "Plains";
	else if (biome == BIOME_TUNDRA)
		return "Tundra";
	else
		return "";
}

//WEATHER------------------------------------------------------------------------------------------------------------------------------------------
RGBA g_skyColor = RGBA(0x00, 0x59, 0x87, 0xff);


//FILE PATHS------------------------------------------------------------------------------------------------------------------------------------------
const char* FOOTSTEP_SOUNDS_PATH = "Data/Sounds/Footsteps/";
const char* DIG_SOUNDS_PATH = "Data/Sounds/Dig/";

//MULTITHREADING------------------------------------------------------------------------------------------------------------------------------------------
CRITICAL_SECTION g_chunkListsCS;
CRITICAL_SECTION g_chunkVertexArrayListsCS;

//RUNNING------------------------------------------------------------------------------------------------------------------------------------------
bool g_isRunning = false;

//Check for working

//Why is it not working