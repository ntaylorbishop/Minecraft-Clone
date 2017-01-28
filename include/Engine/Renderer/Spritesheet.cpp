#define STATIC

#include "Engine/Renderer/SpriteSheet.hpp"

STATIC map<const char*, SpriteSheet*> SpriteSheet::s_spriteSheetRegistry;

//GET SPRITESHEET------------------------------------------------------------------------------------------------------------------------
STATIC SpriteSheet* SpriteSheet::CreateOrGetSpriteSheet(const IntVector2& tileSize, const char* imageFilePath) {
	auto spriteSheet = s_spriteSheetRegistry.find(imageFilePath);

	if (spriteSheet != s_spriteSheetRegistry.end())
		return spriteSheet->second;
	else {
		SpriteSheet* spriteSheet = new SpriteSheet(tileSize, imageFilePath);
		s_spriteSheetRegistry.insert(std::pair<const char*, SpriteSheet*>(imageFilePath, spriteSheet));
		return spriteSheet;
	}
}


//ACCESSORS MUTATORS------------------------------------------------------------------------------------------------------------------------
AABB2 SpriteSheet::GetTexCoordsForSpriteCoords(const IntVector2& spriteCoords) const {
	AABB2 texCoords;
	texCoords.mins.x = m_texCoordsPerTile.x * (float)spriteCoords.x;
	texCoords.maxs.x = m_texCoordsPerTile.x * (float)spriteCoords.x + m_texCoordsPerTile.x;
	texCoords.mins.y = m_texCoordsPerTile.y * (float)spriteCoords.y;
	texCoords.maxs.y = m_texCoordsPerTile.y * (float)spriteCoords.y + m_texCoordsPerTile.y;

	return texCoords;
}

const Texture* SpriteSheet::GetTexture() const {
	return m_spriteSheetTexture;
}

AABB2 SpriteSheet::GetTexCoordsForSpriteIndex(int spriteIndex) const {
	int x = spriteIndex % m_tileSize.x;
	int y = spriteIndex / m_tileSize.y;

	AABB2 texCoords;
	texCoords.mins.x = m_texCoordsPerTile.x * (float)x;
	texCoords.maxs.x = m_texCoordsPerTile.x * (float)x + m_texCoordsPerTile.x;
	texCoords.mins.y = m_texCoordsPerTile.y * (float)y;
	texCoords.maxs.y = m_texCoordsPerTile.y * (float)y + m_texCoordsPerTile.y;

	return texCoords;
}
int SpriteSheet::GetNumSprites() const {
	return m_tileSize.x * m_tileSize.y;
}

//STRUCTORS------------------------------------------------------------------------------------------------------------------------
SpriteSheet::SpriteSheet() {

}
SpriteSheet::SpriteSheet(const IntVector2& tileSize, const char* imageFilePath)
    : m_tileSize(0, 0)
    , m_spriteSheetTexture(nullptr)
    , m_texCoordsPerTile(0.f, 0.f)
{
	m_tileSize = tileSize;
	m_spriteSheetTexture = Texture::CreateOrGetTexture(imageFilePath);
	m_texCoordsPerTile = Vector2(1.f / (float)m_tileSize.x, 1.f / (float)m_tileSize.y);
}