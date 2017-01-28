#include "Game/Chunk.hpp"
#include "Game/TheWorld.hpp"

#define STATIC

//STRUCTORS-------------------------------------------------------------------------------------------------------------------------------------
Chunk::Chunk(const ChunkCoords& chunkPosition)
    : m_chunkPosition(chunkPosition)
    , m_spriteSheet()
    , m_needsToBeChanged(false)
    , m_northNeighbor(nullptr)
    , m_southNeighbor(nullptr)
    , m_eastNeighbor(nullptr)
    , m_westNeighbor(nullptr)
	, m_vboID(0)
	, m_numVertexes(0)
{
	GenerateFromPerlinNoise();
}
Chunk::Chunk(const ChunkCoords& chunkPosition, const vector<unsigned char>& buffer)
    : m_chunkPosition(chunkPosition)
    , m_spriteSheet()
    , m_needsToBeChanged(false)
    , m_northNeighbor(nullptr)
    , m_southNeighbor(nullptr)
    , m_eastNeighbor(nullptr)
    , m_westNeighbor(nullptr)
	, m_vboID(0)
	, m_numVertexes(0)
{
	m_spriteSheet = SpriteSheet::CreateOrGetSpriteSheet(IntVector2(10, 8), SPRITESHEET_TO_USE);
	GenerateFromFile(buffer);
	m_vboID = g_theRenderer->GenerateNewVboID();
	InitializeChunkBiomesAndHeights();
	InitializeLightingColumnHeightsAndSnowTops();
	InitializeWeather();
}
Chunk::~Chunk() {
	g_theRenderer->ClearVBOData(m_vboID);
}

//CHUNK GENERATION-------------------------------------------------------------------------------------------------------------------------------------
void Chunk::InitializeGeneratedChunk() {
	m_spriteSheet = SpriteSheet::CreateOrGetSpriteSheet(IntVector2(10, 8), SPRITESHEET_TO_USE);
	InitializeLightingColumnHeightsAndSnowTops();
	InitializeWeather();
}
void Chunk::GenerateVBOID() {
	m_vboID = g_theRenderer->GenerateNewVboID();
}
void Chunk::GenerateFromPerlinNoise() {
	float columnDifferential[BLOCKS_WIDE_X * BLOCKS_WIDE_Y];
	float columnHumidity[BLOCKS_WIDE_X * BLOCKS_WIDE_Y];
	float columnTemperature[BLOCKS_WIDE_X * BLOCKS_WIDE_Y];
	float columnHilliness[BLOCKS_WIDE_X * BLOCKS_WIDE_Y];

	for (int i = 0; i < BLOCKS_PER_LAYER; i++) {
		ColumnCoords columnWorldCoords = ColumnCoords(m_chunkPosition.x * BLOCKS_WIDE_X, m_chunkPosition.y * BLOCKS_WIDE_Y) + GetColumnCoordsFromIndex(i);
		Vector2 columnCoords = Vector2((float)columnWorldCoords.x, (float)columnWorldCoords.y);

		columnHumidity[i] = ComputePerlinNoise2D(columnCoords, 300.f, 80, 0.5f);
		columnTemperature[i] = ComputePerlinNoise2D(columnCoords, 100.f, 80, 0.5f);
		columnHilliness[i] = 100.f * ComputePerlinNoise2D(columnCoords, 50.f, 50, 0.8f);
		columnDifferential[i] = (100.f * ComputePerlinNoise2D(columnCoords, 200.f, 50, 0.5f));

		if (columnHumidity[i] > 0.f && columnTemperature[i] > 0.f) {
			m_columnBiome[i] = BIOME_PLAINS;
		}
		if (columnHumidity[i] < 0.f && columnTemperature[i] < 0.f) {
			m_columnBiome[i] = BIOME_TUNDRA;
		}
		if (columnHumidity[i] < 0.f && columnTemperature[i] > 0.f) {
			m_columnBiome[i] = BIOME_DESERT;
		}
		if (columnHumidity[i] > 0.f && columnTemperature[i] < 0.f) {
			m_columnBiome[i] = BIOME_PLAINS;
		}
		if (columnDifferential[i] > 18.f)
			m_columnBiome[i] = BIOME_MOUNTAIN;

		m_columnHeight[i] = BASE_HEIGHT + columnDifferential[i];
	}
	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {
		int numColumn = static_cast<int>(i % (BLOCKS_WIDE_Y * BLOCKS_WIDE_X));
		int layer = (i / (BLOCKS_WIDE_Y * BLOCKS_WIDE_X));
		int columnHeight = BASE_HEIGHT + (int)columnDifferential[numColumn];


		if (layer > columnHeight) {
			m_blocks[i] = Block(BLOCK_AIR);
		}
		if (layer == columnHeight) {
			if (m_columnBiome[numColumn] == BIOME_PLAINS)
				m_blocks[i] = Block(BLOCK_GRASS);
			else if (m_columnBiome[numColumn] == BIOME_DESERT)
				m_blocks[i] = Block(BLOCK_SAND);
			else if (m_columnBiome[numColumn] == BIOME_MOUNTAIN)
				m_blocks[i] = Block(BLOCK_SNOW);
			else if (m_columnBiome[numColumn] == BIOME_TUNDRA)
				m_blocks[i] = Block(BLOCK_DIRT);
		}
		if (layer > columnHeight && layer <= SEA_LEVEL) {
			if (m_columnBiome[numColumn] == BIOME_PLAINS)
				m_blocks[i] = Block(BLOCK_WATER);
			else if (m_columnBiome[numColumn] == BIOME_MOUNTAIN)
				m_blocks[i] = Block(BLOCK_ICE);
			else if (m_columnBiome[numColumn] == BIOME_TUNDRA)
				m_blocks[i] = Block(BLOCK_ICE);
			else if (m_columnBiome[numColumn] == BIOME_DESERT)
				m_blocks[i] = Block(BLOCK_SAND);
		}
		if (layer == columnHeight && columnHeight == SEA_LEVEL) {
			if (m_columnBiome[numColumn] == BIOME_PLAINS)
				m_blocks[i] = Block(BLOCK_SAND);
			else if (m_columnBiome[numColumn] == BIOME_DESERT)
				m_blocks[i] = Block(BLOCK_SAND);
			else if (m_columnBiome[numColumn] == BIOME_MOUNTAIN)
				m_blocks[i] = Block(BLOCK_SAND);
			else if (m_columnBiome[numColumn] == BIOME_TUNDRA)
				m_blocks[i] = Block(BLOCK_DIRT);
		}
		if (layer <= columnHeight - 1) {
			if (m_columnBiome[numColumn] == BIOME_PLAINS)
				m_blocks[i] = Block(BLOCK_DIRT);
			else if (m_columnBiome[numColumn] == BIOME_DESERT)
				m_blocks[i] = Block(BLOCK_SAND);
			else if (m_columnBiome[numColumn] == BIOME_MOUNTAIN)
				m_blocks[i] = Block(BLOCK_DIRT);
			else if (m_columnBiome[numColumn] == BIOME_TUNDRA)
				m_blocks[i] = Block(BLOCK_DIRT);
		}
		if (layer < columnHeight - 10) {
			m_blocks[i] = Block(BLOCK_STONE);
		}
	}
}
void Chunk::GenerateFromFile(const vector<unsigned char>& buffer) {
	BlockType currBlockType = BLOCK_AIR;
	int currBlockIdx = 0;

	for (unsigned int i = 0; i < buffer.size(); i++) {
		if (i % 2 == 0) {
			currBlockType = static_cast<BlockType>(buffer[i]);
		}
		else if (i % 2 == 1) {
			//Add to chunk blocks
			for (int j = 0; j < buffer[i]; j++) {
				m_blocks[currBlockIdx + j] = Block(currBlockType);

				if (currBlockType == BLOCK_GLOWSTONE)
					g_theWorld->MarkAsLightingDirty(BlockInfo(this, currBlockIdx + j));
			}
			currBlockIdx += buffer[i];
		}
	}
}
void Chunk::InitializeLightingColumnHeightsAndSnowTops() {
	//FIRST PASS: Descend down each column and update non-opaque blocks to sky
	for (unsigned int column = 0; column < BLOCKS_PER_LAYER; column++) {
		ColumnCoords currColumn = Chunk::GetColumnCoordsFromIndex(column);
		BlockInfo currBlock = BlockInfo(this, GetBlockIndexForLocalCoords(GetTopOfColumnInLocalCoords(currColumn)));

		while (currBlock.GetBlock()) {

			if (!BlockDefinition::IsOpaque(currBlock.GetBlock()->GetBlockType())) {
				currBlock.GetBlock()->SetIsSky();
			}
			else {
				g_theWorld->MarkAsLightingDirty(currBlock);

				//Set min height on column
				m_columnHeight[column] = GetWorldPositionOfBlockInChunk(currBlock.chunk, GetLocalCoordsForBlockIndex(currBlock.m_idx)).z;

				if (currBlock.GetBlock()->GetBlockType() != BLOCK_SNOWTOP && m_columnBiome[column] == BIOME_MOUNTAIN) {
					currBlock = currBlock.GetBlockAbove();
					m_blocks[currBlock.m_idx].SetBlockType(BLOCK_SNOWTOP);
				}
				break;
			}

			currBlock = currBlock.GetBlockBelow();
		}
	}

	//SECOND PASS: Descend down each column and update cardinal neighbors as dirty if need be
	for (unsigned int column = 0; column < BLOCKS_PER_LAYER; column++) {
		ColumnCoords currColumn = Chunk::GetColumnCoordsFromIndex(column);
		BlockInfo currBlock = BlockInfo(this, GetBlockIndexForLocalCoords(GetTopOfColumnInLocalCoords(currColumn)));

		while (currBlock.GetBlock()) {

			if (currBlock.GetBlockToNorth().chunk) {
				BlockInfo northBlock = currBlock.GetBlockToNorth();
				if (northBlock.GetBlock()) {
					if (!BlockDefinition::IsOpaque(northBlock.GetBlock()->GetBlockType()) && !northBlock.GetBlock()->GetIsSky())
						g_theWorld->MarkAsLightingDirty(northBlock);
				}
			}

			if (currBlock.GetBlockToEast().chunk) {
				BlockInfo eastBlock = currBlock.GetBlockToEast();
				if (eastBlock.GetBlock()) {
					if (!BlockDefinition::IsOpaque(eastBlock.GetBlock()->GetBlockType()) && !eastBlock.GetBlock()->GetIsSky())
						g_theWorld->MarkAsLightingDirty(eastBlock);
				}
			}

			if (currBlock.GetBlockToSouth().chunk) {
				BlockInfo southBlock = currBlock.GetBlockToSouth();
				if (southBlock.GetBlock()) {
					if (!BlockDefinition::IsOpaque(southBlock.GetBlock()->GetBlockType()) && !southBlock.GetBlock()->GetIsSky())
						g_theWorld->MarkAsLightingDirty(southBlock);
				}
			}

			if (currBlock.GetBlockToWest().chunk) {
				BlockInfo westBlock = currBlock.GetBlockToWest();
				if (westBlock.GetBlock()) {
					if (!BlockDefinition::IsOpaque(westBlock.GetBlock()->GetBlockType()) && !westBlock.GetBlock()->GetIsSky())
						g_theWorld->MarkAsLightingDirty(westBlock);
				}
			}

			if (currBlock.GetBlockBelow().GetBlock() && !BlockDefinition::IsOpaque(currBlock.GetBlockBelow().GetBlock()->GetBlockType())) {
				currBlock = currBlock.GetBlockBelow();
			}
			else
				break;
		}
	}
}

void Chunk::InitializeChunkBiomesAndHeights() {
	float columnDifferential[BLOCKS_WIDE_X * BLOCKS_WIDE_Y];
	float columnHumidity[BLOCKS_WIDE_X * BLOCKS_WIDE_Y];
	float columnTemperature[BLOCKS_WIDE_X * BLOCKS_WIDE_Y];

	for (int i = 0; i < BLOCKS_PER_LAYER; i++) {
		ColumnCoords columnWorldCoords = ColumnCoords(m_chunkPosition.x * BLOCKS_WIDE_X, m_chunkPosition.y * BLOCKS_WIDE_Y) + GetColumnCoordsFromIndex(i);
		Vector2 columnCoords = Vector2((float)columnWorldCoords.x, (float)columnWorldCoords.y);

		columnHumidity[i] = ComputePerlinNoise2D(columnCoords, 300.f, 80, 0.5f);
		columnTemperature[i] = ComputePerlinNoise2D(columnCoords, 100.f, 80, 0.5f);
		columnDifferential[i] = (100.f * ComputePerlinNoise2D(columnCoords, 200.f, 50, 0.5f));

		if (columnHumidity[i] > 0.f && columnTemperature[i] > 0.f) {
			m_columnBiome[i] = BIOME_PLAINS;
		}
		if (columnHumidity[i] < 0.f && columnTemperature[i] < 0.f) {
			m_columnBiome[i] = BIOME_TUNDRA;
		}
		if (columnHumidity[i] < 0.f && columnTemperature[i] > 0.f) {
			m_columnBiome[i] = BIOME_DESERT;
		}
		if (columnHumidity[i] > 0.f && columnTemperature[i] < 0.f) {
			m_columnBiome[i] = BIOME_PLAINS;
		}
		if (columnDifferential[i] > 18.f)
			m_columnBiome[i] = BIOME_MOUNTAIN;

		m_columnHeight[i] = BASE_HEIGHT + columnDifferential[i];
	}
}
void Chunk::InitializeWeather() {
	for (int i = 0; i < BLOCKS_PER_LAYER; i++) {
		ColumnCoords columnWorldCoords = ColumnCoords(m_chunkPosition.x * BLOCKS_WIDE_X, m_chunkPosition.y * BLOCKS_WIDE_Y) + GetColumnCoordsFromIndex(i);
		WorldPosition worldCoords = WorldPosition((float)columnWorldCoords.x, (float)columnWorldCoords.y, 0.f);

		if (m_columnBiome[i] == BIOME_MOUNTAIN) {
			m_snowSheets.push_back(WeatherSheet(worldCoords, WEATHER_SNOW, m_columnHeight[i]));
		}
		else if (m_columnBiome[i] == BIOME_PLAINS) {
			m_rainSheets.push_back(WeatherSheet(worldCoords, WEATHER_RAIN, m_columnHeight[i]));
		}
	}
}

//UPDATE-----------------------------------------------------------------------------------------------------------------------------------------
bool Chunk::Update() {
	if (m_needsToBeChanged) {
		//UpdateSnow();
		ComputeChunkVertices();
		m_needsToBeChanged = false;
		return true;
	}
	else
		return false;
}
void Chunk::UpdateWeather(float deltaSeconds) {
	for (unsigned int i = 0; i < m_snowSheets.size(); i++) {
		m_snowSheets[i].Update(deltaSeconds);
	}

	for (unsigned int i = 0; i < m_rainSheets.size(); i++) {
		m_rainSheets[i].Update(deltaSeconds);
	}
}
void Chunk::UpdateSnow() {
	for (unsigned int column = 0; column < BLOCKS_PER_LAYER; column++) {
		ColumnCoords currColumn = Chunk::GetColumnCoordsFromIndex(column);
		BlockInfo currBlock = BlockInfo(this, GetBlockIndexForLocalCoords(GetTopOfColumnInLocalCoords(currColumn)));

		while (currBlock.GetBlock()) {

			if (currBlock.GetBlockBelow().GetBlock()) {

				if (!BlockDefinition::IsOpaque(currBlock.GetBlockBelow().GetBlock()->GetBlockType()) || currBlock.GetBlockBelow().GetBlock()->GetBlockType() == BLOCK_SNOWTOP) {
					continue;
				}
				else {
					m_blocks[currBlock.m_idx] = Block(BLOCK_SNOWTOP);
					break;
				}
			}

			currBlock = currBlock.GetBlockBelow();
		}
	}
}

void Chunk::PlaceBlock(const WorldPosition& blockPos, BlockType type) {
	LocalCoords newBlockPos = GetLocalCoordsForWorldCoords(blockPos);
	int idxToPlaceBlock = GetBlockIndexForLocalCoords(newBlockPos);
	BlockInfo currBlock = BlockInfo(this, idxToPlaceBlock);

	//Update lighting below this block if it's sky
	if (currBlock.GetBlock() && currBlock.GetBlock()->GetIsSky()) {

		while (currBlock.GetBlock()) {
			if (!BlockDefinition::IsOpaque(currBlock.GetBlock()->GetBlockType())) {
				currBlock.GetBlock()->SetIsNotSky();
			}

			g_theWorld->MarkAsLightingDirty(currBlock);
			currBlock = currBlock.GetBlockBelow();
		}
	}

	m_blocks[GetBlockIndexForLocalCoords(newBlockPos)].SetBlockType(type);
	m_needsToBeChanged = true;

	BlockInfo bi = BlockInfo(this, idxToPlaceBlock);
	g_theWorld->MarkAsLightingDirty(bi);
}
void Chunk::DeleteBlock(const WorldPosition& blockPos) {
	LocalCoords blockToDeleteBlock = GetLocalCoordsForWorldCoords(blockPos);
	int idxToDeleteBlock = GetBlockIndexForLocalCoords(blockToDeleteBlock);

	m_blocks[idxToDeleteBlock].SetBlockType(BLOCK_AIR);
	m_needsToBeChanged = true;

	//Update lighting below this block
	BlockInfo currBlock = BlockInfo(this, idxToDeleteBlock);

	if (currBlock.GetBlockAbove().GetBlock() && currBlock.GetBlockAbove().GetBlock()->GetIsSky()) {
			
		while (currBlock.GetBlock()) {
			if (!BlockDefinition::IsOpaque(currBlock.GetBlock()->GetBlockType())) {
				currBlock.GetBlock()->SetIsSky();
				g_theWorld->MarkAsLightingDirty(currBlock);
			}
			else {
				g_theWorld->MarkAsLightingDirty(currBlock);
				break;
			}

			currBlock = currBlock.GetBlockBelow();
		}
	}

	g_theWorld->MarkAsLightingDirty(currBlock);

	//Check if we need to update neighbor chunk vertex arrays
	if (IsBlockPositionOnNorthBorderOfChunk(blockToDeleteBlock))
		m_northNeighbor->MarkVertexArrayDirty(true);

	if (IsBlockPositionOnSouthBorderOfChunk(blockToDeleteBlock))
		m_southNeighbor->MarkVertexArrayDirty(true);

	if (IsBlockPositionOnEastBorderOfChunk(blockToDeleteBlock))
		m_eastNeighbor->MarkVertexArrayDirty(true);

	if (IsBlockPositionOnWestBorderOfChunk(blockToDeleteBlock))
		m_westNeighbor->MarkVertexArrayDirty(true);
}

//RENDER------------------------------------------------------------------------------------------------------------------------------------------
void Chunk::RenderWeather() const {
	vector<TexturedVertex> allSnowSheetVerts;
	vector<TexturedVertex> allRainSheetVerts;

	for (unsigned int i = 0; i < m_snowSheets.size(); i++) {
		const vector<TexturedVertex>* currSnowSheetVerts = m_snowSheets[i].GetVerts();
		allSnowSheetVerts.insert(allSnowSheetVerts.end(), currSnowSheetVerts->begin(), currSnowSheetVerts->end());
	}

	for (unsigned int i = 0; i < m_rainSheets.size(); i++) {
		const vector<TexturedVertex>* currRainSheetVerts = m_rainSheets[i].GetVerts();
		allRainSheetVerts.insert(allRainSheetVerts.end(), currRainSheetVerts->begin(), currRainSheetVerts->end());
	}

	g_theRenderer->Enable(GL_ALPHA_TEST);
	g_theRenderer->Disable(GL_CULL_FACE);
	if (allSnowSheetVerts.size() != 0) {
		g_theRenderer->BindAndEnableTexture(m_snowSheets[0].GetSnowTexture());
		g_theRenderer->DrawVertexArray(allSnowSheetVerts, allSnowSheetVerts.size(), PRIMITIVE_QUADS);
		g_theRenderer->UnbindAndDisableTexture(m_snowSheets[0].GetSnowTexture());
	}

	if (allRainSheetVerts.size() != 0) {
		g_theRenderer->BindAndEnableTexture(m_rainSheets[0].GetRainTexture());
		g_theRenderer->DrawVertexArray(allRainSheetVerts, allRainSheetVerts.size(), PRIMITIVE_QUADS);
		g_theRenderer->UnbindAndDisableTexture(m_rainSheets[0].GetRainTexture());
	}
	g_theRenderer->Enable(GL_CULL_FACE);
}

void Chunk::MarkVertexArrayDirty(bool isDirty) {
	m_needsToBeChanged = isDirty;
}

bool Chunk::IsChunkVisible(const WorldPosition& cameraDirection, const WorldPosition& cameraPos) {
	WorldPosition bottom1 = GetWorldPositionForChunkCoords(m_chunkPosition);
	WorldPosition bottom2 = GetWorldPositionForChunkCoords(m_chunkPosition) + WorldPosition(0.f,           BLOCKS_WIDE_Y, 0.f);
	WorldPosition bottom3 = GetWorldPositionForChunkCoords(m_chunkPosition) + WorldPosition(BLOCKS_WIDE_X, BLOCKS_WIDE_Y, 0.f);
	WorldPosition bottom4 = GetWorldPositionForChunkCoords(m_chunkPosition) + WorldPosition(BLOCKS_WIDE_X, 0.f,           0.f);

	WorldPosition top1 = GetWorldPositionForChunkCoords(m_chunkPosition)    + WorldPosition(0.f,           0.f,           BLOCKS_WIDE_Z);
	WorldPosition top2 = GetWorldPositionForChunkCoords(m_chunkPosition)    + WorldPosition(0.f,           BLOCKS_WIDE_Y, BLOCKS_WIDE_Z);
	WorldPosition top3 = GetWorldPositionForChunkCoords(m_chunkPosition)    + WorldPosition(BLOCKS_WIDE_X, BLOCKS_WIDE_Y, BLOCKS_WIDE_Z);
	WorldPosition top4 = GetWorldPositionForChunkCoords(m_chunkPosition)    + WorldPosition(BLOCKS_WIDE_X, 0.f,           BLOCKS_WIDE_Z);

	WorldPosition camToBottom1 = bottom1 - cameraPos;
	WorldPosition camToBottom2 = bottom2 - cameraPos;
	WorldPosition camToBottom3 = bottom3 - cameraPos;
	WorldPosition camToBottom4 = bottom4 - cameraPos;
	WorldPosition camToTop1    = top1 - cameraPos;
	WorldPosition camToTop2    = top2 - cameraPos;
	WorldPosition camToTop3    = top3 - cameraPos;
	WorldPosition camToTop4    = top4 - cameraPos;

	vector<float> dotProductsOfCorners;

	dotProductsOfCorners.push_back(DotProduct(camToBottom1, cameraDirection));
	dotProductsOfCorners.push_back(DotProduct(camToBottom2, cameraDirection));
	dotProductsOfCorners.push_back(DotProduct(camToBottom3, cameraDirection));
	dotProductsOfCorners.push_back(DotProduct(camToBottom4, cameraDirection));
	dotProductsOfCorners.push_back(DotProduct(camToTop1, cameraDirection));
	dotProductsOfCorners.push_back(DotProduct(camToTop2, cameraDirection));
	dotProductsOfCorners.push_back(DotProduct(camToTop3, cameraDirection));
	dotProductsOfCorners.push_back(DotProduct(camToTop4, cameraDirection));

	for (unsigned int i = 0; i < dotProductsOfCorners.size(); i++) {
		if (dotProductsOfCorners[i] > 0.f)
			return true;
	}

	return false;
}

int Chunk::GetVBOID() {
	return m_vboID;
}
int Chunk::GetNumVertexes() {
	return m_numVertexes;
}
BIOME Chunk::GetColumnBiome(const WorldPosition& worldPos) {
	LocalCoords loc = GetLocalCoordsForWorldCoords(worldPos);
	int blockIdx = GetBlockIndexForLocalCoords(loc);
	ColumnCoords column = GetColumnCoordsFromIndex(blockIdx);
	int idx = column.x * column.y;

	return m_columnBiome[idx];
}


//PRIVATE UTILITY------------------------------------------------------------------------------------------------------------------------------
void Chunk::ComputeChunkVertices() {

	static Vector3 corner1 = Vector3(0.f, 0.f, 0.f);
	static Vector3 corner2 = Vector3(1.f, 0.f, 0.f);
	static Vector3 corner3 = Vector3(0.f, 1.f, 0.f);
	static Vector3 corner4 = Vector3(0.f, 0.f, 1.f);
	static Vector3 corner5 = Vector3(1.f, 1.f, 0.f);
	static Vector3 corner6 = Vector3(1.f, 0.f, 1.f);
	static Vector3 corner7 = Vector3(0.f, 1.f, 1.f);
	static Vector3 corner8 = Vector3(1.f, 1.f, 1.f);

	static Vector3 snowTopCorner6 = Vector3(1.f, 0.f, 0.2f);
	static Vector3 snowTopCorner7 = Vector3(0.f, 1.f, 0.2f);
	static Vector3 snowTopCorner8 = Vector3(1.f, 1.f, 0.2f);
	static Vector3 snowTopCorner4 = Vector3(0.f, 0.f, 0.2f);

	vector<TexturedVertex> vertices;
	vertices.reserve(40000);

	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {
		LocalCoords localCoordsFromIdx = GetLocalCoordsForBlockIndex(i);
		Vector3 localCoordsInWorldCoords = Vector3((float)localCoordsFromIdx.x, (float)localCoordsFromIdx.y, (float)localCoordsFromIdx.z);
		Vector3 pos = Vector3((float)m_chunkPosition.x * (float)BLOCKS_WIDE_X, (float)m_chunkPosition.y * (float)BLOCKS_WIDE_Y, 0.f) + localCoordsInWorldCoords;
		BlockInfo thisBlockInfo = TheWorld::GetBlockInfoForWorldPosition(pos);

		Block block = m_blocks[i];
		BlockType blockType = block.GetBlockType();

		if (blockType != BLOCK_AIR) {
			IntVector2 spriteBottom = BlockDefinition::SpriteBottom(blockType);
			SpriteCoords spriteCoords_Bottom = m_spriteSheet->GetTexCoordsForSpriteCoords(spriteBottom);
			IntVector2 spriteTop = BlockDefinition::SpriteTop(blockType);
			SpriteCoords spriteCoords_Top = m_spriteSheet->GetTexCoordsForSpriteCoords(spriteTop);
			IntVector2 spriteSides = BlockDefinition::SpriteSides(blockType);
			SpriteCoords spriteCoords_Sides = m_spriteSheet->GetTexCoordsForSpriteCoords(spriteSides);
			RGBA blockTint = BlockDefinition::BlockTint(blockType);

			//FRONT FACE
			Vector3 bottomLeft = corner3 + pos;
			Vector3 bottomRight = corner1 + pos;
			Vector3 topRight = corner4 + pos;
			Vector3 topLeft = corner7 + pos;

			if (blockType == BLOCK_SNOWTOP) {
				topRight = snowTopCorner4 + pos;
				topLeft = snowTopCorner7 + pos;
			}

			if (IsFaceShowing(i, FACE_NORTH)) {
				BlockInfo blockNorth = thisBlockInfo.GetBlockToNorth();
				if (blockNorth.chunk)
					blockTint = blockNorth.GetBlock()->BlockTint();
				PushVertices(bottomLeft, bottomRight, topRight, topLeft, spriteCoords_Sides, blockTint, vertices);
			}

			//BACK FACE
			bottomLeft = corner2 + pos;
			bottomRight = corner5 + pos;
			topRight = corner8 + pos;
			topLeft = corner6 + pos;

			if (blockType == BLOCK_SNOWTOP) {
				topRight = snowTopCorner8 + pos;
				topLeft = snowTopCorner6 + pos;
			}


			if (IsFaceShowing(i, FACE_SOUTH)) {
				BlockInfo blockSouth = thisBlockInfo.GetBlockToSouth();
				if (blockSouth.chunk)
					blockTint = blockSouth.GetBlock()->BlockTint();
				PushVertices(bottomLeft, bottomRight, topRight, topLeft, spriteCoords_Sides, blockTint, vertices);
			}

			//BOTTOM FACE
			bottomLeft = corner1 + pos;
			bottomRight = corner3 + pos;
			topRight = corner5 + pos;
			topLeft = corner2 + pos;

			if (IsFaceShowing(i, FACE_BOTTOM)) {
				BlockInfo blockBelow = thisBlockInfo.GetBlockBelow();
				if (blockBelow.chunk)
					blockTint = blockBelow.GetBlock()->BlockTint();
				PushVertices(bottomLeft, bottomRight, topRight, topLeft, spriteCoords_Bottom, blockTint, vertices);
			}

			//TOP FACE
			if (blockType != BLOCK_SNOWTOP) {
				bottomLeft = corner7 + pos;
				bottomRight = corner4 + pos;
				topRight = corner6 + pos;
				topLeft = corner8 + pos;
			}
			else {
				bottomLeft  = snowTopCorner7 + pos;
				bottomRight = snowTopCorner4 + pos;
				topRight    = snowTopCorner6 + pos;
				topLeft     = snowTopCorner8 + pos;
			}

			if (IsFaceShowing(i, FACE_TOP)) {
				BlockInfo blockAbove = thisBlockInfo.GetBlockAbove();
				if (blockAbove.chunk)
					blockTint = blockAbove.GetBlock()->BlockTint();
				PushVertices(bottomLeft, bottomRight, topRight, topLeft, spriteCoords_Top, blockTint, vertices);
			}

			//RIGHT FACE
			bottomLeft = corner1 + pos;
			bottomRight = corner2 + pos;
			topRight = corner6 + pos;
			topLeft = corner4 + pos;

			if (blockType == BLOCK_SNOWTOP) {
				topRight = snowTopCorner6 + pos;
				topLeft = snowTopCorner4 + pos;
			}

			if (IsFaceShowing(i, FACE_WEST)) {
				BlockInfo blockWest = thisBlockInfo.GetBlockToWest();
				if (blockWest.chunk)
					blockTint = blockWest.GetBlock()->BlockTint();
				PushVertices(bottomLeft, bottomRight, topRight, topLeft, spriteCoords_Sides, blockTint, vertices);
			}

			//LEFT FACE
			bottomLeft = corner5 + pos;
			bottomRight = corner3 + pos;
			topRight = corner7 + pos;
			topLeft = corner8 + pos;

			if (blockType == BLOCK_SNOWTOP) {
				topRight = snowTopCorner7 + pos;
				topLeft = snowTopCorner8 + pos;
			}

			if (IsFaceShowing(i, FACE_EAST)) {
				BlockInfo blockEast = thisBlockInfo.GetBlockToEast();
				if (blockEast.chunk)
					blockTint = blockEast.GetBlock()->BlockTint();
				PushVertices(bottomLeft, bottomRight, topRight, topLeft, spriteCoords_Sides, blockTint, vertices);
			}
		}
	}

	g_theRenderer->UpdateVBO(vertices, vertices.size(), m_vboID);
	m_numVertexes = vertices.size();
}

bool Chunk::IsFaceShowing(int idx, BlockFace face) {
	BlockInfo currBlock = g_theWorld->GetBlockInfoForWorldPosition(GetWorldPositionOfBlock(idx));
	BlockInfo blockNextToThisBlock;

	if (face == FACE_NORTH) {
		blockNextToThisBlock = currBlock.GetBlockToNorth();
	}
	else if (face == FACE_EAST) {
		blockNextToThisBlock = currBlock.GetBlockToEast();
	}
	else if (face == FACE_SOUTH) {
		blockNextToThisBlock = currBlock.GetBlockToSouth();
	}
	else if (face == FACE_WEST) {
		blockNextToThisBlock = currBlock.GetBlockToWest();
	}
	else if (face == FACE_TOP) {
		blockNextToThisBlock = currBlock.GetBlockAbove();
	}
	else if (face == FACE_BOTTOM) {
		blockNextToThisBlock = currBlock.GetBlockBelow();
	}

	if (blockNextToThisBlock.chunk) {
		BlockType adjacentBlockType = blockNextToThisBlock.GetBlock()->GetBlockType();

		if (currBlock.GetBlock()->GetBlockType() == BLOCK_WATER) {
			return (face == FACE_TOP);
		}
		else {
			return (!BlockDefinition::IsOpaque(adjacentBlockType));
		}
	}
	else
		return false;
}
void Chunk::PushVertices(const Vector3& bottomLeft, const Vector3& bottomRight, const Vector3& topRight, const Vector3& topLeft, const SpriteCoords& spriteCoords, const RGBA& tint, vector<TexturedVertex>& vertices) {
	TexCoords bottomLeft_TC = TexCoords(spriteCoords.mins.x, spriteCoords.maxs.y);
	TexCoords bottomRight_TC = TexCoords(spriteCoords.maxs.x, spriteCoords.maxs.y);
	TexCoords topRight_TC = TexCoords(spriteCoords.maxs.x, spriteCoords.mins.y);
	TexCoords topLeft_TC = TexCoords(spriteCoords.mins.x, spriteCoords.mins.y);

	vertices.push_back(TexturedVertex(bottomRight, tint, bottomRight_TC));
	vertices.push_back(TexturedVertex(topRight, tint, topRight_TC));
	vertices.push_back(TexturedVertex(topLeft, tint, topLeft_TC));
	vertices.push_back(TexturedVertex(bottomLeft, tint, bottomLeft_TC));
}