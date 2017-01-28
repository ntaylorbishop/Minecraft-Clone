#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <string>
#include <map>

#include "Engine/Renderer/OpenGLExtensions.hpp"
#include "Engine/Math/IntVector2.hpp"

using namespace std;
class Texture {
public:
	static Texture* Texture::CreateOrGetTexture(const string& imageFilePath);
	static Texture* Texture::GetTextureByName(const string& imageFilePath);

	int getTextureID() const;
	
private:
	Texture(const string& imageFilePath);

	static map<string, Texture*> s_textureRegistry;
	GLuint m_openglTextureID;
	IntVector2 m_texelSize;
};
