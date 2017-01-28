#include "Game/TheWorld.hpp"
#include "Game/ThePlayer.hpp"

#define STATIC

TheWorld* g_theWorld = nullptr;

STATIC deque<ChunkCoords> TheWorld::m_chunkRequests;
STATIC set<Chunk*> TheWorld::m_finishedGenChunks;

//STRUCTORS------------------------------------------------------------------------------------------------------------------
TheWorld::TheWorld() 
    : m_spriteSheet(nullptr)
	, m_closestWeatherSheet(200.f)
{
	InitializeThreads();
	InitializeWorld();
}
TheWorld::~TheWorld() {

	if (DEBUG_EnableFileSaveLoad) {
		for (auto it = m_activeChunks.begin(); it != m_activeChunks.end(); ++it) {
			ChunkCoords key = it->first;
			Chunk* chunk = it->second;

			string fileName = "Data/SaveSlot0/(" + to_string(chunk->GetChunkPosition().x) + "," + to_string(chunk->GetChunkPosition().y) + ").chunk";
			vector<unsigned char> buffer;

			unsigned char numBlocksOfCurrType = 0;
			unsigned char currBlockType = 0;
			unsigned char prevBlockType = 0;
			for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {
				if (i == 0) {
					currBlockType = (unsigned char)chunk->m_blocks[i].GetBlockType();
					prevBlockType = (unsigned char)chunk->m_blocks[i].GetBlockType();
					numBlocksOfCurrType++;
				}
				else {
					prevBlockType = currBlockType;
					currBlockType = (unsigned char)chunk->m_blocks[i].GetBlockType();

					if (numBlocksOfCurrType == 255) {
						buffer.push_back(prevBlockType);
						buffer.push_back(numBlocksOfCurrType);
						numBlocksOfCurrType = 0;
					}

					if (prevBlockType != currBlockType) {
						buffer.push_back(prevBlockType);
						buffer.push_back(numBlocksOfCurrType);
						numBlocksOfCurrType = 0;
					}
					numBlocksOfCurrType++;
				}
			}

			if (!SaveBufferToBinaryFile(fileName, buffer))
				SystemDialogue_Okay("ERROR: SAVING CHUNK TO FILE", "File couldn't be opened.", SEVERITY_INFORMATION);
		}
	}

	BlockDefinition::FlushBlockDefinitions();

	delete g_theCloudSystem;
	g_theCloudSystem = nullptr;
}

//INITIALIZATION------------------------------------------------------------------------------------------------------------------
void TheWorld::InitializeThreads() {
	InitializeCriticalSection(&g_chunkListsCS);

	thread genThread(ChunkGenerationThreadMain);
	genThread.detach();
}
void TheWorld::InitializeWorld() {
	m_spriteSheet = SpriteSheet::CreateOrGetSpriteSheet(IntVector2(10, 8), SPRITESHEET_TO_USE);
	BlockDefinition::InitializeBlockDefinitions();
	g_theCloudSystem = new TheCloudSystem();
}

//THREADING------------------------------------------------------------------------------------------------------------------
STATIC void TheWorld::ChunkGenerationThreadMain() {
	while (!g_theInputSystem->HasQuit()) {
		ChunkCoords newChunkCoords;
		bool gotCoords = false;

		EnterCriticalSection(&g_chunkListsCS);
		if (!m_chunkRequests.empty()) {
			gotCoords = true;
			newChunkCoords = m_chunkRequests.front();
			m_chunkRequests.pop_front();
		}
		LeaveCriticalSection(&g_chunkListsCS);

		if (!gotCoords) {
			continue;
		}

		Chunk* newChunk = new Chunk(newChunkCoords);
		newChunk->MarkVertexArrayDirty(true);

		EnterCriticalSection(&g_chunkListsCS);
		m_finishedGenChunks.insert(newChunk);
		LeaveCriticalSection(&g_chunkListsCS);
	}
}

//UPDATE CYCLE------------------------------------------------------------------------------------------------------------------
void TheWorld::Update(const WorldPosition& playerPos, float deltaSeconds) {
	ChunkCoords center = Chunk::GetChunkCoordsForWorldCoords(playerPos);

	AddGeneratedChunks();

	if (!UpdateAVertexArrayIfDirty()) {
		if (!RegisterAndConnectOneChunk(center))
			DeregisterAndDeconnectOneChunk(center);
	}
	UpdateLighting();
	UpdateWeather(deltaSeconds);
	UpdateSkyForWeather();
	ModulateSkyValueAndPlayRainSound();

	g_theCloudSystem->Update(deltaSeconds);
}

void TheWorld::UpdateWeather(float deltaSeconds) {
	//Update every chunk's weather
	for (auto iterator = m_activeChunks.begin(); iterator != m_activeChunks.end(); iterator++) {
		Chunk* chunk = iterator->second;
		chunk->UpdateWeather(deltaSeconds);
	}
}

void TheWorld::UpdateSkyForWeather() {
	BlockInfo bi = GetBlockInfoForWorldPosition(WorldPosition(g_thePlayer->m_position.x, g_thePlayer->m_position.y, 0.f));
	float closestDist = -1.f;
	int currRadius = 1;

	while (currRadius < 20) {

		for (int x = -currRadius; x < currRadius; x++) {
			for (int y = -currRadius; y < currRadius; y++) {
				WorldPosition currPos = WorldPosition(g_thePlayer->m_position.x + x, g_thePlayer->m_position.y + y, g_thePlayer->m_position.z);
				bi = GetBlockInfoForWorldPosition(currPos);

				if (bi.chunk && bi.chunk->GetColumnBiome(currPos) == BIOME_PLAINS) {
					Vector2 playerPos2D = Vector2(g_thePlayer->m_position.x, g_thePlayer->m_position.y);
					Vector2 currPos2D = Vector2(currPos.x, currPos.y);

					float dist = (currPos2D - playerPos2D).length();
					if (dist < closestDist || closestDist == -1.f) {
						closestDist = dist;
					}
				}
			}
		}

		currRadius++;
	}

	m_closestWeatherSheet = closestDist;
}
void TheWorld::ModulateSkyValueAndPlayRainSound() {
	if (m_closestWeatherSheet < 5.f && m_closestWeatherSheet != -1.f) {
		float fractionOfDist = m_closestWeatherSheet / 5.f;
		if (fractionOfDist < 0.1f)
			fractionOfDist = 0.f;

		float rMapped = RangeMap(fractionOfDist, 0.f, 1.f, SKY_COLOR_RAIN.r(), SKY_COLOR_REGULAR.r());
		float gMapped = RangeMap(fractionOfDist, 0.f, 1.f, SKY_COLOR_RAIN.g(), SKY_COLOR_REGULAR.g());
		float bMapped = RangeMap(fractionOfDist, 0.f, 1.f, SKY_COLOR_RAIN.b(), SKY_COLOR_REGULAR.b());

		g_skyColor = RGBA(rMapped, gMapped, bMapped, 1.f);

		DebuggerPrintf("%f\n", 1.f - fractionOfDist);
		g_theAudioPlayer->PlayRainSound();
		g_theAudioPlayer->ChangeVolumeOfRainSound(1.f - fractionOfDist);
	}
	else {
		g_skyColor = SKY_COLOR_REGULAR;

		g_theAudioPlayer->StopRain();
	}
}

void TheWorld::UpdateLighting() {
	while(!m_dirtyBlocks.empty()) {
		BlockInfo bi = m_dirtyBlocks.front();
		m_dirtyBlocks.pop_front();
		bi.GetBlock()->SetIsNotDirty();
		UpdateLightingForBlock(bi);
	}
}
bool TheWorld::UpdateAVertexArrayIfDirty() {
	bool foundAChunkToUpdate = false;

	for (auto iterator = m_activeChunks.begin(); iterator != m_activeChunks.end(); iterator++) {
		Chunk* chunk = iterator->second;
		if (chunk->Update()) {
			foundAChunkToUpdate = true;
			break;
		}
	}

	return foundAChunkToUpdate;
}

//LIGHTING------------------------------------------------------------------------------------------------------------------
void TheWorld::UpdateLightingForBlock(BlockInfo bi) {
	int ideal = CalcIdealLightForBlock(bi); //Sets internal light value for block
	int current = bi.GetBlock()->GetLightLevel();
	if(ideal == current)
		return;

	bi.GetBlock()->SetLightLevel(ideal);
	SetBlockNeighborsDirty(bi);
	bi.chunk->MarkVertexArrayDirty(true);
}
int TheWorld::CalcIdealLightForBlock(BlockInfo bi) {
	int myLight = BlockDefinition::InternalLightValue(bi.GetBlock()->GetBlockType());

	//Get Highest Neighbor Minus One
	int eastNeighborLightLevel = 0;
	int westNeighborLightLevel = 0;
	int southNeighborLightLevel = 0;
	int northNeighborLightLevel = 0;
	int aboveNeighborLightLevel = 0;
	int belowNeighborLightLevel = 0;

	if (bi.GetBlockToEast().GetBlock() != nullptr)
		eastNeighborLightLevel = bi.GetBlockToEast().GetBlock()->GetLightLevel();

	if (bi.GetBlockToWest().GetBlock() != nullptr)
		westNeighborLightLevel = bi.GetBlockToWest().GetBlock()->GetLightLevel();

	if (bi.GetBlockToNorth().GetBlock() != nullptr)
		northNeighborLightLevel = bi.GetBlockToNorth().GetBlock()->GetLightLevel();

	if (bi.GetBlockToSouth().GetBlock() != nullptr)
		southNeighborLightLevel = bi.GetBlockToSouth().GetBlock()->GetLightLevel();

	if (bi.GetBlockAbove().GetBlock() != nullptr)
		aboveNeighborLightLevel = bi.GetBlockAbove().GetBlock()->GetLightLevel();

	if (bi.GetBlockBelow().GetBlock() != nullptr)
		belowNeighborLightLevel = bi.GetBlockBelow().GetBlock()->GetLightLevel();

	int highestNeighborMinusOne = max( max(aboveNeighborLightLevel, max(eastNeighborLightLevel, westNeighborLightLevel)), max( belowNeighborLightLevel, max(southNeighborLightLevel, northNeighborLightLevel))) - 1;

	int skyLight = bi.GetBlock()->GetIsSky() ? SKY_LIGHT_LEVEL : 0;
	return max(max(myLight, highestNeighborMinusOne), skyLight);
}
void TheWorld::MarkAsLightingDirty(BlockInfo bi) {
	Block* block = bi.GetBlock();
	if (!block || block->IsDirty())
		return;

	m_dirtyBlocks.push_back(bi);
	block->SetIsDirty();
}
void TheWorld::SetBlockNeighborsDirty(BlockInfo bi) {
	if (bi.GetBlockToEast().GetBlock() != nullptr) {
		if (!BlockDefinition::IsOpaque(bi.GetBlockToEast().GetBlock()->GetBlockType()))
			MarkAsLightingDirty(bi.GetBlockToEast());
	}
	if (bi.GetBlockToWest().GetBlock() != nullptr) {
		if (!BlockDefinition::IsOpaque(bi.GetBlockToWest().GetBlock()->GetBlockType()))
			MarkAsLightingDirty(bi.GetBlockToWest());
	}
	if (bi.GetBlockToNorth().GetBlock() != nullptr) {
		if (!BlockDefinition::IsOpaque(bi.GetBlockToNorth().GetBlock()->GetBlockType()))
			MarkAsLightingDirty(bi.GetBlockToNorth());
	}
	if (bi.GetBlockToSouth().GetBlock() != nullptr) {
		if (!BlockDefinition::IsOpaque(bi.GetBlockToSouth().GetBlock()->GetBlockType()))
			MarkAsLightingDirty(bi.GetBlockToSouth());
	}
	if (bi.GetBlockAbove().GetBlock() != nullptr) {
		if (!BlockDefinition::IsOpaque(bi.GetBlockAbove().GetBlock()->GetBlockType()))
			MarkAsLightingDirty(bi.GetBlockAbove());
	}
	if (bi.GetBlockBelow().GetBlock() != nullptr) {
		if (!BlockDefinition::IsOpaque(bi.GetBlockBelow().GetBlock()->GetBlockType()))
			MarkAsLightingDirty(bi.GetBlockBelow());
	}
}

//RENDER------------------------------------------------------------------------------------------------------------------
void TheWorld::Render(const WorldPosition& cameraDirection, const WorldPosition& cameraPos) const {

	//Get chunk vertex arrays and render
	for (auto iterator = m_activeChunks.begin(); iterator != m_activeChunks.end(); iterator++) {
		Chunk* chunk = iterator->second;

		if (chunk->IsChunkVisible(cameraDirection, cameraPos)) {
			int vboID = chunk->GetVBOID();

			g_theRenderer->BindAndEnableTexture(const_cast<Texture*>(m_spriteSheet->GetTexture()));
			g_theRenderer->DrawVBO(chunk->GetNumVertexes(), PRIMITIVE_QUADS, vboID, true);
			g_theRenderer->UnbindAndDisableTexture(const_cast<Texture*>(m_spriteSheet->GetTexture()));

			chunk->RenderWeather();
		}
	}

	g_theCloudSystem->Render();
}

//CHUNK GENERATION------------------------------------------------------------------------------------------------------------------
void TheWorld::AddGeneratedChunks() {
	bool foundGeneratedChunk = false;
	Chunk* chunkToAdd = nullptr;

	//THREADED CHUNK GENERATION
	EnterCriticalSection(&g_chunkListsCS);
	if (!m_finishedGenChunks.empty()) {
		chunkToAdd = *m_finishedGenChunks.begin();
		m_finishedGenChunks.erase(m_finishedGenChunks.begin());
		foundGeneratedChunk = true;
	}
	LeaveCriticalSection(&g_chunkListsCS);

	if (foundGeneratedChunk) {
		ChunkCoords chunkCoordsOfChunkToAdd = chunkToAdd->GetChunkPosition();
		m_activeChunks.insert(m_activeChunks.begin(), pair<ChunkCoords, Chunk*>(chunkCoordsOfChunkToAdd, chunkToAdd));
		chunkToAdd->GenerateVBOID();
		chunkToAdd->InitializeGeneratedChunk();

		UpdateNeighbors(chunkCoordsOfChunkToAdd);

		g_theCloudSystem->InitializeCloudsForChunk(chunkToAdd);
	}
}
bool TheWorld::RegisterAndConnectOneChunk(const ChunkCoords& playerCenter) {
	ChunkCoords closestChunkToAdd;
	ChunkCoords currChunk;

	//Find a chunk to add
	for (int x = playerCenter.x - CHUNK_GENERATION_INNER_RADIUS; x < playerCenter.x + CHUNK_GENERATION_INNER_RADIUS; x++) {
		for (int y = playerCenter.y - CHUNK_GENERATION_INNER_RADIUS; y < playerCenter.y + CHUNK_GENERATION_INNER_RADIUS; y++) {
			currChunk = ChunkCoords(x, y);

			if (x == playerCenter.x - CHUNK_GENERATION_INNER_RADIUS && y == playerCenter.y - CHUNK_GENERATION_INNER_RADIUS) {
				closestChunkToAdd = ChunkCoords(x, y);
			}
			else {
				if (m_activeChunks.find(currChunk) == m_activeChunks.end()) {
					int distToPlayerOfCurrChunk = DistanceSquaredBetweenPoints(playerCenter, currChunk);
					int distToPlayerOfClosestChunk = DistanceSquaredBetweenPoints(playerCenter, closestChunkToAdd);

					if (distToPlayerOfCurrChunk < CHUNK_GENERATION_INNER_RADIUS && distToPlayerOfCurrChunk < distToPlayerOfClosestChunk) {
						closestChunkToAdd = currChunk;
					}
				}
			}
		}
	}

	//Pull from file, else generate from perlin noise
	if (closestChunkToAdd != ChunkCoords(playerCenter.x - CHUNK_GENERATION_INNER_RADIUS, playerCenter.y - CHUNK_GENERATION_INNER_RADIUS)) { //Make sure we found a chunk to add
		vector<unsigned char> buffer;
		string fileName = "Data/SaveSlot0/(" + to_string(closestChunkToAdd.x) + "," + to_string(closestChunkToAdd.y) + ").chunk";

		if (DEBUG_EnableFileSaveLoad && LoadBinaryFileToBuffer(fileName, buffer)) {
			Chunk* chunkToAdd = new Chunk(closestChunkToAdd, buffer);
			m_activeChunks.insert(m_activeChunks.begin(), pair<ChunkCoords, Chunk*>(closestChunkToAdd, chunkToAdd));
			chunkToAdd->MarkVertexArrayDirty(true);
			g_theCloudSystem->InitializeCloudsForChunk(chunkToAdd);

			UpdateNeighbors(closestChunkToAdd);
		}
		else {
			//THREADED CHUNK GENERATION

			EnterCriticalSection(&g_chunkListsCS);
			m_chunkRequests.push_back(closestChunkToAdd);
			LeaveCriticalSection(&g_chunkListsCS);
			return false;
		}

		return true;
	}
	else
		return false;
}


bool TheWorld::DeregisterAndDeconnectOneChunk(const ChunkCoords& playerCenter) {
	for (auto it = m_activeChunks.begin(); it != m_activeChunks.end(); ++it) {
		ChunkCoords key = it->first;

		float distToPlayer = (float)DistanceSquaredBetweenPoints(playerCenter, key);

		if (distToPlayer >= CHUNK_GENERATION_OUTER_RADIUS) {
			Chunk* chunk = it->second;

			if (DEBUG_EnableFileSaveLoad) {
				string fileName = "Data/SaveSlot0/(" + to_string(chunk->GetChunkPosition().x) + "," + to_string(chunk->GetChunkPosition().y) + ").chunk";
				vector<unsigned char> buffer;

				unsigned char numBlocksOfCurrType = 0;
				unsigned char currBlockType = 0;
				unsigned char prevBlockType = 0;
				for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {
					if (i == 0) {
						currBlockType = (unsigned char)chunk->m_blocks[i].GetBlockType();
						prevBlockType = (unsigned char)chunk->m_blocks[i].GetBlockType();
						numBlocksOfCurrType++;
					}
					else {
						prevBlockType = currBlockType;
						currBlockType = (unsigned char)chunk->m_blocks[i].GetBlockType();

						if (numBlocksOfCurrType == 255) {
							buffer.push_back(prevBlockType);
							buffer.push_back(numBlocksOfCurrType);
							numBlocksOfCurrType = 0;
						}

						if (prevBlockType != currBlockType) {
							buffer.push_back(prevBlockType);
							buffer.push_back(numBlocksOfCurrType);
							numBlocksOfCurrType = 0;
						}
						numBlocksOfCurrType++;
					}
				}
				
				if (DEBUG_EnableFileSaveLoad) {
					if (!SaveBufferToBinaryFile(fileName, buffer))
						SystemDialogue_Okay("ERROR: SAVING CHUNK TO FILE", "File couldn't be opened.", SEVERITY_INFORMATION);
				}
			}
			g_theCloudSystem->FlushCloudsForChunk(chunk);
			delete chunk;
			m_activeChunks.erase(key);
			UpdateNeighbors(key);
			return true;
		}
	}
	return false;
}

void TheWorld::UpdateNeighbors(const ChunkCoords& chunkLoc) {
	ChunkCoords northNeighborLoc = chunkLoc + ChunkCoords(-1, 0);
	ChunkCoords eastNeighborLoc = chunkLoc + ChunkCoords(0, 1);
	ChunkCoords southNeighborLoc = chunkLoc + ChunkCoords(1, 0);
	ChunkCoords westNeighborLoc = chunkLoc + ChunkCoords(0, -1);

	auto northChunkIter = m_activeChunks.find(northNeighborLoc);
	auto eastChunkIter = m_activeChunks.find(eastNeighborLoc);
	auto southChunkIter = m_activeChunks.find(southNeighborLoc);
	auto westChunkIter = m_activeChunks.find(westNeighborLoc);

	Chunk* northNeighbor = northChunkIter == m_activeChunks.end() ? nullptr : northChunkIter->second;
	Chunk* eastNeighbor = eastChunkIter == m_activeChunks.end() ? nullptr : eastChunkIter->second;
	Chunk* southNeighbor = southChunkIter == m_activeChunks.end() ? nullptr : southChunkIter->second;
	Chunk* westNeighbor = westChunkIter == m_activeChunks.end() ? nullptr : westChunkIter->second;

	Chunk* thisChunk = nullptr;
	auto found = m_activeChunks.find(chunkLoc);

	if (found != m_activeChunks.end())
		thisChunk = found->second;

	//UPDATE NORTH NEIGHBOR
	if (thisChunk) {
		thisChunk->m_northNeighbor = northNeighbor;
		thisChunk->m_eastNeighbor = eastNeighbor;
		thisChunk->m_southNeighbor = southNeighbor;
		thisChunk->m_westNeighbor = westNeighbor;
		thisChunk->MarkVertexArrayDirty(true);
	}

	if (northNeighbor) {
		northNeighbor->m_southNeighbor = thisChunk;
		northNeighbor->MarkVertexArrayDirty(true);
	}
	if (eastNeighbor) {
		eastNeighbor->m_westNeighbor = thisChunk;
		eastNeighbor->MarkVertexArrayDirty(true);
	}
	if (southNeighbor) {
		southNeighbor->m_northNeighbor = thisChunk;
		southNeighbor->MarkVertexArrayDirty(true);
	}
	if (westNeighbor) {
		westNeighbor->m_eastNeighbor = thisChunk;
		westNeighbor->MarkVertexArrayDirty(true);
	}
}

//COORDINATE SWITCHING------------------------------------------------------------------------------------------------------------------
Block* TheWorld::GetBlockForWorldCoords(const WorldPosition& position) {
	ChunkCoords key = Chunk::GetChunkCoordsForWorldCoords(position);

	auto it = m_activeChunks.find(key);
	if (it == m_activeChunks.end())
		return nullptr;
	else {
		Chunk* chunk = it->second;

		LocalCoords blockPosition = Chunk::GetLocalCoordsForWorldCoords(position);

		if (chunk->IsBlockPositionWithinBounds(blockPosition)) {
			return &chunk->m_blocks[chunk->GetBlockIndexForLocalCoords(blockPosition)];
		}
		else
			return nullptr;
	}
}
STATIC BlockInfo TheWorld::GetBlockInfoForWorldPosition(const WorldPosition& worldPos) {
	ChunkCoords key = Chunk::GetChunkCoordsForWorldCoords(worldPos);
	BlockInfo bi = BlockInfo();

	auto it = g_theWorld->m_activeChunks.find(key);
	if (it != g_theWorld->m_activeChunks.end()) {
		bi.chunk = it->second;

		LocalCoords blockPosition = Chunk::GetLocalCoordsForWorldCoords(worldPos);

		if (bi.chunk->IsBlockPositionWithinBounds(blockPosition)) //sanity check
			bi.m_idx = Chunk::GetBlockIndexForLocalCoords(blockPosition);
	}
	return bi;
}
STATIC BlockInfo TheWorld::GetBlockInfoForLocalCoords(const LocalCoords& loc) {
	WorldPosition locInWorldCoords = WorldPosition((float)loc.x, (float)loc.y, (float)loc.z);
	return GetBlockInfoForWorldPosition(locInWorldCoords);
}