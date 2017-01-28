#pragma once

#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Renderer/RGBA.hpp"

class TexturedVertex {
public:
	TexturedVertex();
	TexturedVertex(const Vector3& pos, const RGBA& col, const Vector2& tex);

	Vector3 m_position;
	RGBA m_color;
	Vector2 m_texCoords;
};

class Vertex {
public:
	Vertex();
	Vertex(const Vector3& pos, const RGBA& col);

	Vector3 m_position;
	RGBA m_color;
};