#include "Game/TheCloudSystem.hpp"
#include "Game/Chunk.hpp"

TheCloudSystem* g_theCloudSystem = nullptr;

TheCloudSystem::TheCloudSystem() 
    : m_clouds()
{ }

void TheCloudSystem::InitializeCloudsForChunk(Chunk* chunk) {
	WorldPosition newCloudPos = chunk->GetChunkWorldPosition();
	float possibleYPos = GetNumberForRandomSeed(RandInt(0, 5), 0);
	float possibleXPos = GetNumberForRandomSeed(RandInt(0, 5), 1);
	newCloudPos += WorldPosition(possibleXPos, possibleYPos, CLOUD_HEIGHT);

	Vector3 newCloudSize = Vector3(Randf(CLOUD_X_MIN_SIZE, CLOUD_X_MAX_SIZE), Randf(CLOUD_Y_MIN_SIZE, CLOUD_Y_MAX_SIZE), Randf(CLOUD_Z_MIN_SIZE, CLOUD_Z_MAX_SIZE));

	float newCloudSpeed = CLOUD_MIN_SPEED;
	Cloud* cloud = new Cloud(newCloudPos, newCloudSize, newCloudSpeed);

	m_clouds.insert(m_clouds.begin(), pair<ChunkCoords, Cloud*>(chunk->GetChunkPosition(), cloud));
}
void TheCloudSystem::FlushCloudsForChunk(Chunk* chunk) {
	map<ChunkCoords, Cloud*>::iterator itr = m_clouds.begin();
	while (itr != m_clouds.end()) {
		Cloud* currCloud = itr->second;

		Vector3 start = chunk->GetChunkWorldPosition();
		Vector3 end = start + Vector3(BLOCKS_WIDE_X, BLOCKS_WIDE_Y, 0.f);

		if (NumberWithinRange(currCloud->m_position.x, start.x, end.x) && NumberWithinRange(currCloud->m_position.y, start.y, end.y)) {
			map<ChunkCoords, Cloud*>::iterator toErase = itr;
			++itr;
			m_clouds.erase(toErase);
		}
		else {
			++itr;
		}
	}
}

void TheCloudSystem::Update(float deltaSeconds) {
	m_vertices.clear();

	for (auto it = m_clouds.begin(); it != m_clouds.end(); ++it) {
		Cloud* cloud = it->second;

		cloud->UpdateCloudSpeed(deltaSeconds);
		ComputeCloudVertices(cloud);
	}
}

void TheCloudSystem::Render() const {
	if (m_vertices.size() != 0) {
		g_theRenderer->Disable(GL_CULL_FACE);
		g_theRenderer->EnableGLAlphaFunc(0.4f);
		g_theRenderer->DrawVertexArray(m_vertices, m_vertices.size(), PRIMITIVE_QUADS);
	}
}

void TheCloudSystem::ComputeCloudVertices(Cloud* cloud) {
	static RGBA cloudTint_TopBottom = RGBA(0.7f, 0.7f, 0.7f, 1.f);
	static RGBA cloudTint_FrontBack = RGBA(0.6f, 0.6f, 0.6f, 1.f);
	static RGBA cloudTint_LeftRight = RGBA(0.5f, 0.5f, 0.5f, 1.f);


	//BOTTOM FACE
	Vector3 topLeft = cloud->m_position + Vector3(-cloud->m_size.x, cloud->m_size.y, -cloud->m_size.z);
	Vector3 topRight = cloud->m_position + Vector3(cloud->m_size.x, cloud->m_size.y, -cloud->m_size.z);
	Vector3 bottomRight = cloud->m_position + Vector3(cloud->m_size.x, -cloud->m_size.y, -cloud->m_size.z);
	Vector3 bottomLeft = cloud->m_position + Vector3(-cloud->m_size.x, -cloud->m_size.y, -cloud->m_size.z);

	m_vertices.push_back(Vertex(topLeft, cloudTint_TopBottom));
	m_vertices.push_back(Vertex(topRight, cloudTint_TopBottom));
	m_vertices.push_back(Vertex(bottomRight, cloudTint_TopBottom));
	m_vertices.push_back(Vertex(bottomLeft, cloudTint_TopBottom));

	//TOP FACE
	topLeft = cloud->m_position + Vector3(-cloud->m_size.x, cloud->m_size.y, cloud->m_size.z);
	topRight = cloud->m_position + Vector3(cloud->m_size.x, cloud->m_size.y, cloud->m_size.z);
	bottomRight = cloud->m_position + Vector3(cloud->m_size.x, -cloud->m_size.y, cloud->m_size.z);
	bottomLeft = cloud->m_position + Vector3(-cloud->m_size.x, -cloud->m_size.y, cloud->m_size.z);

	m_vertices.push_back(Vertex(topLeft, cloudTint_TopBottom));
	m_vertices.push_back(Vertex(topRight, cloudTint_TopBottom));
	m_vertices.push_back(Vertex(bottomRight, cloudTint_TopBottom));
	m_vertices.push_back(Vertex(bottomLeft, cloudTint_TopBottom));

	//FRONT FACE
	topLeft = cloud->m_position + Vector3(-cloud->m_size.x, -cloud->m_size.y, cloud->m_size.z);
	topRight = cloud->m_position + Vector3(cloud->m_size.x, -cloud->m_size.y, cloud->m_size.z);
	bottomRight = cloud->m_position + Vector3(cloud->m_size.x, -cloud->m_size.y, -cloud->m_size.z);
	bottomLeft = cloud->m_position + Vector3(-cloud->m_size.x, -cloud->m_size.y, -cloud->m_size.z);

	m_vertices.push_back(Vertex(topLeft, cloudTint_FrontBack));
	m_vertices.push_back(Vertex(topRight, cloudTint_FrontBack));
	m_vertices.push_back(Vertex(bottomRight, cloudTint_FrontBack));
	m_vertices.push_back(Vertex(bottomLeft, cloudTint_FrontBack));

	//BACK FACE
	topLeft = cloud->m_position + Vector3(-cloud->m_size.x, cloud->m_size.y, cloud->m_size.z);
	topRight = cloud->m_position + Vector3(cloud->m_size.x, cloud->m_size.y, cloud->m_size.z);
	bottomRight = cloud->m_position + Vector3(cloud->m_size.x, cloud->m_size.y, -cloud->m_size.z);
	bottomLeft = cloud->m_position + Vector3(-cloud->m_size.x, cloud->m_size.y, -cloud->m_size.z);

	m_vertices.push_back(Vertex(topLeft, cloudTint_FrontBack));
	m_vertices.push_back(Vertex(topRight, cloudTint_FrontBack));
	m_vertices.push_back(Vertex(bottomRight, cloudTint_FrontBack));
	m_vertices.push_back(Vertex(bottomLeft, cloudTint_FrontBack));

	//LEFT FACE
	topLeft = cloud->m_position + Vector3(-cloud->m_size.x, -cloud->m_size.y, cloud->m_size.z);
	topRight = cloud->m_position + Vector3(-cloud->m_size.x, cloud->m_size.y, cloud->m_size.z);
	bottomRight = cloud->m_position + Vector3(-cloud->m_size.x, cloud->m_size.y, -cloud->m_size.z);
	bottomLeft = cloud->m_position + Vector3(-cloud->m_size.x, -cloud->m_size.y, -cloud->m_size.z);

	m_vertices.push_back(Vertex(topLeft, cloudTint_LeftRight));
	m_vertices.push_back(Vertex(topRight, cloudTint_LeftRight));
	m_vertices.push_back(Vertex(bottomRight, cloudTint_LeftRight));
	m_vertices.push_back(Vertex(bottomLeft, cloudTint_LeftRight));

	//RIGHT FACE
	topLeft = cloud->m_position + Vector3(cloud->m_size.x, -cloud->m_size.y, cloud->m_size.z);
	topRight = cloud->m_position + Vector3(cloud->m_size.x, cloud->m_size.y, cloud->m_size.z);
	bottomRight = cloud->m_position + Vector3(cloud->m_size.x, cloud->m_size.y, -cloud->m_size.z);
	bottomLeft = cloud->m_position + Vector3(cloud->m_size.x, -cloud->m_size.y, -cloud->m_size.z);

	m_vertices.push_back(Vertex(topLeft, cloudTint_LeftRight));
	m_vertices.push_back(Vertex(topRight, cloudTint_LeftRight));
	m_vertices.push_back(Vertex(bottomRight, cloudTint_LeftRight));
	m_vertices.push_back(Vertex(bottomLeft, cloudTint_LeftRight));
}
float TheCloudSystem::GetNumberForRandomSeed(int randomNumber, int whichAxis) {
	switch (whichAxis) {
	case 1:
		return ((float)randomNumber * CLOUD_Y_MAX_SIZE + 2.f);
	case 2: 
		return ((float)randomNumber * CLOUD_X_MAX_SIZE + 2.f);
	default:
		return -1;
	}
}