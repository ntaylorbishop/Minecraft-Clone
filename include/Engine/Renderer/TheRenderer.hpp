#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <vector>
#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library
#pragma comment( lib, "GLu32" ) // Link in the GLu32.lib static library

#include "Engine/Renderer/OpenGLExtensions.hpp"
#include "Engine/Renderer/Vertex.hpp"
#include "Engine/Renderer/Spritesheet.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/RGBA.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/IntVector3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/ErrorWarningAssert.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/AABB3.hpp"

typedef Vector2 TexCoords;

enum PrimitiveType {
	PRIMITIVE_POINTS = 0,
	PRIMITIVE_LINES,
	PRIMITIVE_LINE_LOOP,
	PRIMITIVE_TRIANGLES,
	PRIMITIVE_QUADS,
	NUM_PRIMITIVE_TYPES
};

class TheRenderer;
extern TheRenderer* g_theRenderer;

using namespace std;
class TheRenderer {
public:
	//STRUCTORS
	TheRenderer();

	//INITIALIZATION
	void Initialize();

	//SETUP / UTILITY FUNCS
	void Enable(GLenum cap);
	void Disable(GLenum cap);
	void BlendMode(GLenum sfactor, GLenum dfactor);
	void PointSize(float size);
	void LineWidth(float width);
	void ClearBuffer(GLbitfield mask);

	void ClearScreen(float red, float green, float blue);

	//DRAW OBJECTS
	void BindAndEnableTexture(Texture* texture);
	void UnbindAndDisableTexture(Texture* texture);

	void DrawLine(const Vector2& start, const Vector2& end, RGBA color);
	void DrawLine(const Vector3& start, const Vector3& end, RGBA color);
	void DrawCircle(const Vector2& center, float radius, RGBA color);

	//DRAW AABB's
	void DrawAABB2(const AABB2& quad, const RGBA& tint, PrimitiveType type);

	void DrawTexturedAABB2(Texture* texture, const RGBA& tint, const AABB2& quad);

	void DrawTexturedAABB2FromSpriteSheet(const SpriteSheet* spriteSheet, const RGBA& tint, const AABB2& quad, const AABB2& texCoords);
	void DrawTexturedAABB2FromSpriteSheet(const SpriteSheet* spriteSheet, const RGBA& tint, const AABB2& quad, const IntVector2& spriteCoords);
	void DrawTexturedQuadFromSpritesheet(const SpriteSheet* spriteSheet, const RGBA& tint, const Vector3& bottomLeft, const Vector3& bottomRight, const Vector3& topRight, const Vector3& topLeft, const IntVector2& spriteCoords);

	//VERTEX ARRAYS
	void DrawVertexArray(const Vertex* verts, int numVerts, PrimitiveType type);
	void DrawVertexArray(const vector<Vertex>& verts, int numVerts, PrimitiveType type);
	void DrawVertexArray(const TexturedVertex* verts, int numVerts, PrimitiveType type);
	void DrawVertexArray(const vector<TexturedVertex>& verts, int numVerts, PrimitiveType type);

	//VBO'S
	int GenerateNewVboID();

	void UpdateVBO(const Vertex* verts, int numVerts, int vboID);
	void UpdateVBO(const vector<Vertex>& verts, int numVerts, int vboID);
	void UpdateVBO(const TexturedVertex* verts, int numVerts, int vboID);
	void UpdateVBO(const vector<TexturedVertex>& verts, int numVerts, int vboID);

	void DrawVBO(int numVerts, PrimitiveType type, int &vboID, bool isTextured);

	void ClearVBOData(int vboID);

	//VIEW MATRIX TRANSLATIONS
	void LoadIdentity();
	void SetOrtho2D(const Vector2& mins, const Vector2& maxs);
	void SetPerspective(float fieldOfViewYAxis, float aspect, float nearClippingPlane, float farClippingPlane);

	void TranslateViewMatrix(const Vector2& offset);
	void TranslateViewMatrix(const Vector3& offset);
	void RotateViewMatrix(float angle); //Takes radians
	void RotateViewMatrix(float angle, const Vector3& axisOfRotation); //Takes degrees
	void PushViewMatrix();
	void PopViewMatrix();
	void ScaleViewMatrix2D(Vector2 scale);
	void ScaleViewMatrix3D(Vector3 scale);

	//DRAWING / COLORING FUNCS
	void MotionBlur();
	void ClearColor(const RGBA& color);
	void SetColor(const RGBA& color);

	//DEBUG DRAWING
	void AddDebugPoint(const Vector3& position, const RGBA& color);
	void DrawDebugPoints(float pointSize, bool enableDepthTesting);
	void ClearDebugPoints();

	GLenum GetGLEnumForPrimitiveType(PrimitiveType type);

	//TESTING
	void EnableGLAlphaFunc(float amount);
	void DisableGLAlphaFunc();

private:
	RGBA m_colorMode;
	vector<Vertex> m_debugPoints;
};