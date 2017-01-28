#include "Engine/Renderer/Vertex.hpp"

//TEXTURED VERTEX
TexturedVertex::TexturedVertex()
    : m_position(Vector3(0.f, 0.f, 0.f))
    , m_color(RGBA(0.f, 0.f, 0.f, 0.f))
    , m_texCoords(0.f, 0.f)
{}
TexturedVertex::TexturedVertex(const Vector3& pos, const RGBA& col, const Vector2& tex)
    : m_position(pos)
    , m_color(col)
    , m_texCoords(tex)
{}

//VERTEX
Vertex::Vertex()
    : m_position()
    , m_color()
{}
Vertex::Vertex(const Vector3& pos, const RGBA& col)
    : m_position(pos)
    , m_color(col)
{}