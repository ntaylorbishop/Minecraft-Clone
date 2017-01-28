#include "Engine/Renderer/BitmapFont.hpp"

#define STATIC //do-nothing static indicator

STATIC map<string, BitmapFont*> BitmapFont::s_fontRegistry;

STATIC BitmapFont* BitmapFont::CreateOrGetFont(const std::string& bitmapFontName) {
	auto font = s_fontRegistry.find(bitmapFontName);

	if (font != s_fontRegistry.end())
		return font->second;
	else if(bitmapFontName == "SquirrelFixedFont") {
		BitmapFont* font = new BitmapFont(bitmapFontName);

		s_fontRegistry.insert(pair<string, BitmapFont*>(bitmapFontName, new BitmapFont(bitmapFontName)));
		return font;
	}
	else {
		DebuggerPrintf("Font does not exist. Creating default font (SquirrelFixedFont)");

		BitmapFont* font = new BitmapFont("SquirrelFixedFont");

		s_fontRegistry.insert(pair<string, BitmapFont*>(bitmapFontName, new BitmapFont(bitmapFontName)));
		return font;
	}
}
AABB2 BitmapFont::GetTexCoordsForGlyph(int glyphUnicode) const {
	return m_spriteSheet->GetTexCoordsForSpriteIndex(glyphUnicode);
}

void BitmapFont::DrawText2D(const Vector2& startBottomLeft, const std::string& asciiText, float cellHeight, const RGBA& tint, const BitmapFont* font) {
	if (font == NULL) {
		auto fontIterator = s_fontRegistry.find("SquirrelFixedFont");
	}
	
	for (unsigned int i = 0; i < asciiText.size(); i++) {
		char charToDraw = asciiText[i];

		AABB2 quadToDrawOn = AABB2(startBottomLeft + Vector2((float)(i * cellHeight), 0.f), startBottomLeft + Vector2((float)(i * cellHeight + cellHeight), cellHeight));
		AABB2 texCoords = m_spriteSheet->GetTexCoordsForSpriteIndex((int)charToDraw);

		g_theRenderer->DrawTexturedAABB2FromSpriteSheet(m_spriteSheet, tint, quadToDrawOn, texCoords);
	}
}

BitmapFont::BitmapFont(const string& bitmapFontName) {
	m_spriteSheet = SpriteSheet::CreateOrGetSpriteSheet(IntVector2(16, 16), m_squirrelFixedFont);
}