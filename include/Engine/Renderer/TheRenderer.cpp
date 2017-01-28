#include "Engine/Renderer/TheRenderer.hpp"

TheRenderer* g_theRenderer = nullptr;

//STRUCTORS
TheRenderer::TheRenderer() 
: m_colorMode(RGBA::WHITE)
, m_debugPoints()
{
	Initialize();
}

//INITIALIZATION
void TheRenderer::Initialize() {
	glGenBuffers     = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glBindBuffer     = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData     = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
	glDeleteBuffers  = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
}

//----------------------------------SETUP / UTILITY FUNCS--------------------------------------
void TheRenderer::Enable(GLenum cap) {
	glEnable(cap);
}
void TheRenderer::Disable(GLenum cap) {
	glDisable(cap);
}
void TheRenderer::BlendMode(GLenum sfactor, GLenum dfactor) {
	glBlendFunc(sfactor, dfactor);
}
void TheRenderer::PointSize(float size) {
	glPointSize(size);
}
void TheRenderer::LineWidth(float width) {
	glLineWidth(width);
}
void TheRenderer::ClearBuffer(GLbitfield mask) {
	glClear(mask);
}

void TheRenderer::ClearScreen(float red, float green, float blue) {
	glClearColor(red, green, blue, 1.f);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------DRAW OBJECTS--------------------------------------
void TheRenderer::BindAndEnableTexture(Texture* texture) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
}
void TheRenderer::UnbindAndDisableTexture(Texture* texture) {
	glDisable(GL_TEXTURE_2D);
}
void TheRenderer::DrawLine(const Vector2& start, const Vector2& end, RGBA color) {

	Vertex* verts = new Vertex[2];

	verts[0].m_position = Vector3(start.x, start.y, 0.f);
	verts[0].m_color = color;

	verts[1].m_position = Vector3(end.x, end.y, 0.f);
	verts[1].m_color = color;

	DrawVertexArray(verts, 2, PRIMITIVE_LINES);

	delete verts;
}
void TheRenderer::DrawLine(const Vector3& start, const Vector3& end, RGBA color) {

	Vertex* verts = new Vertex[2];

	verts[0].m_position = start;
	verts[0].m_color = color;

	verts[1].m_position = end;
	verts[1].m_color = color;

	DrawVertexArray(verts, 2, PRIMITIVE_LINES);

	delete verts;
}
void TheRenderer::DrawCircle(const Vector2& center, float radius, RGBA color) {
	const float numSides = 50.f;
	const float radiansTotal = 2.f * pi;
	const float radiansPerSide = radiansTotal / numSides;

	vector<Vertex> verts;

	for (float radians = 0.f + 2.f; radians < radiansTotal + 2.f; radians += radiansPerSide) {
		float x = (cos(radians) * radius) + center.x;
		float y = (sin(radians) * radius) + center.y;

		verts.push_back(Vertex(Vector3(x, y, 0.f), color));
	}

	DrawVertexArray(verts, verts.size(), PRIMITIVE_LINE_LOOP);
}

void TheRenderer::DrawAABB2(const AABB2& quad, const RGBA& tint, PrimitiveType type) {

	Vertex* verts = new Vertex[4];

	verts[0].m_color = tint;
	verts[0].m_position = Vector3(quad.mins.x, quad.mins.y, 0.f);

	verts[1].m_color = tint;
	verts[1].m_position = Vector3(quad.maxs.x, quad.mins.y, 0.f);

	verts[2].m_color = tint;
	verts[2].m_position = Vector3(quad.maxs.x, quad.maxs.y, 0.f);

	verts[3].m_color = tint;
	verts[3].m_position = Vector3(quad.mins.x, quad.maxs.y, 0.f);

	DrawVertexArray(verts, 4, type);

	delete verts;
}

//----------------------------------DRAW AABB's--------------------------------------
void TheRenderer::DrawTexturedAABB2(Texture* texture, const RGBA& tint, const AABB2& quad) {
	
	TexturedVertex* verts = new TexturedVertex[4];

	verts[0].m_texCoords = Vector2(0.f, 1.f);
	verts[0].m_color = tint;
	verts[0].m_position = Vector3(quad.mins.x, quad.mins.y, 0.f);

	verts[1].m_texCoords = Vector2(1.f, 1.f);
	verts[1].m_color = tint;
	verts[1].m_position = Vector3(quad.maxs.x, quad.mins.y, 0.f);

	verts[2].m_texCoords = Vector2(1.f, 0.f);
	verts[2].m_color = tint;
	verts[2].m_position = Vector3(quad.maxs.x, quad.maxs.y, 0.f);

	verts[3].m_texCoords = Vector2(0.f, 0.f);
	verts[3].m_color = tint;
	verts[3].m_position = Vector3(quad.mins.x, quad.maxs.y, 0.f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());

	DrawVertexArray(verts, 4, PRIMITIVE_QUADS);

	glDisable(GL_TEXTURE_2D);

	delete verts;
}
void TheRenderer::DrawTexturedAABB2FromSpriteSheet(const SpriteSheet* spriteSheet, const RGBA& tint, const AABB2& quad, const AABB2& texCoords) {
	const Texture* textureToDrawFrom = spriteSheet->GetTexture();

	TexturedVertex* verts = new TexturedVertex[4];

	verts[0].m_texCoords = Vector2(texCoords.mins.x, texCoords.maxs.y);
	verts[0].m_color = tint;
	verts[0].m_position = Vector3(quad.mins.x, quad.mins.y, 0.f);

	verts[1].m_texCoords = Vector2(texCoords.maxs.x, texCoords.maxs.y);
	verts[1].m_color = tint;
	verts[1].m_position = Vector3(quad.maxs.x, quad.mins.y, 0.f);

	verts[2].m_texCoords = Vector2(texCoords.maxs.x, texCoords.mins.y);
	verts[2].m_color = tint;
	verts[2].m_position = Vector3(quad.maxs.x, quad.maxs.y, 0.f);

	verts[3].m_texCoords = Vector2(texCoords.mins.x, texCoords.mins.y);
	verts[3].m_color = tint;
	verts[3].m_position = Vector3(quad.mins.x, quad.maxs.y, 0.f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureToDrawFrom->getTextureID());

	DrawVertexArray(verts, 4, PRIMITIVE_QUADS);

	glDisable(GL_TEXTURE_2D);

	delete verts;
}
void TheRenderer::DrawTexturedAABB2FromSpriteSheet(const SpriteSheet* spriteSheet, const RGBA& tint, const AABB2& quad, const IntVector2& spriteCoords) {
	AABB2 texCoords = spriteSheet->GetTexCoordsForSpriteCoords(spriteCoords);
	const Texture* textureToDrawFrom = spriteSheet->GetTexture();

	TexturedVertex* verts = new TexturedVertex[4];

	verts[0].m_texCoords = Vector2(texCoords.mins.x, texCoords.maxs.y);
	verts[0].m_color = tint;
	verts[0].m_position = Vector3(quad.mins.x, quad.mins.y, 0.f);

	verts[1].m_texCoords = Vector2(texCoords.maxs.x, texCoords.maxs.y);
	verts[1].m_color = tint;
	verts[1].m_position = Vector3(quad.maxs.x, quad.mins.y, 0.f);

	verts[2].m_texCoords = Vector2(texCoords.maxs.x, texCoords.mins.y);
	verts[2].m_color = tint;
	verts[2].m_position = Vector3(quad.maxs.x, quad.maxs.y, 0.f);

	verts[3].m_texCoords = Vector2(texCoords.mins.x, texCoords.mins.y);
	verts[3].m_color = tint;
	verts[3].m_position = Vector3(quad.mins.x, quad.maxs.y, 0.f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureToDrawFrom->getTextureID());

	DrawVertexArray(verts, 4, PRIMITIVE_QUADS);

	glDisable(GL_TEXTURE_2D);

	delete verts;
}

void TheRenderer::DrawTexturedQuadFromSpritesheet(const SpriteSheet* spriteSheet, const RGBA& tint, const Vector3& bottomLeft, const Vector3& bottomRight, const Vector3& topRight, const Vector3& topLeft, const IntVector2& spriteCoords) {
	AABB2 texCoords = spriteSheet->GetTexCoordsForSpriteCoords(spriteCoords);
	const Texture* textureToDrawFrom = spriteSheet->GetTexture();

	TexturedVertex* verts = new TexturedVertex[4];

	verts[0].m_texCoords = Vector2(texCoords.maxs.x, texCoords.maxs.y);
	verts[0].m_color = tint;
	verts[0].m_position = Vector3(bottomRight.x, bottomRight.y, bottomRight.z);

	verts[1].m_texCoords = Vector2(texCoords.maxs.x, texCoords.mins.y);
	verts[1].m_color = tint;
	verts[1].m_position = Vector3(topRight.x, topRight.y, topRight.z);

	verts[2].m_texCoords = Vector2(texCoords.mins.x, texCoords.mins.y);
	verts[2].m_color = tint;
	verts[2].m_position = Vector3(topLeft.x, topLeft.y, topLeft.z);

	verts[3].m_texCoords = Vector2(texCoords.mins.x, texCoords.maxs.y);
	verts[3].m_color = tint;
	verts[3].m_position = Vector3(bottomLeft.x, bottomLeft.y, bottomLeft.z);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureToDrawFrom->getTextureID());

	DrawVertexArray(verts, 4, PRIMITIVE_QUADS);

	glDisable(GL_TEXTURE_2D);

	delete verts;
}

//VERTEX ARRAYS----------------------------------------------------------------------------------
void TheRenderer::DrawVertexArray(const Vertex* verts, int numVerts, PrimitiveType type) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &verts[0].m_position);
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), &verts[0].m_color);

	glDrawArrays(GetGLEnumForPrimitiveType(type), 0, numVerts);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}
void TheRenderer::DrawVertexArray(const vector<Vertex>& verts, int numVerts, PrimitiveType type) {
	DrawVertexArray(&verts[0], numVerts, type);
}
void TheRenderer::DrawVertexArray(const TexturedVertex* verts, int numVerts, PrimitiveType type) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(TexturedVertex), &verts[0].m_position);
	glColorPointer(4, GL_FLOAT, sizeof(TexturedVertex), &verts[0].m_color);
	glTexCoordPointer(2, GL_FLOAT, sizeof(TexturedVertex), &verts[0].m_texCoords);

	glDrawArrays(GetGLEnumForPrimitiveType(type), 0, numVerts);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
void TheRenderer::DrawVertexArray(const vector<TexturedVertex>& verts, int numVerts, PrimitiveType type) {
	DrawVertexArray(&verts[0], numVerts, type);
}

//VBO's----------------------------------------------------------------------------------
int TheRenderer::GenerateNewVboID() {
	GLuint newID = 0;
	glGenBuffers(1, &newID);

	return (int)newID;
}

void TheRenderer::UpdateVBO(const Vertex* verts, int numVerts, int vboID) {

	size_t vertexArrayNumBytes = sizeof(Vertex) * numVerts;
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, vertexArrayNumBytes, verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void TheRenderer::UpdateVBO(const vector<Vertex>& verts, int numVerts, int vboID) {

	size_t vertexArrayNumBytes = sizeof(Vertex)* numVerts;
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, vertexArrayNumBytes, verts.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void TheRenderer::UpdateVBO(const TexturedVertex* verts, int numVerts, int vboID) {

	size_t vertexArrayNumBytes = sizeof(TexturedVertex)* numVerts;
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, vertexArrayNumBytes, verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void TheRenderer::UpdateVBO(const vector<TexturedVertex>& verts, int numVerts, int vboID) {

	size_t vertexArrayNumBytes = sizeof(TexturedVertex)* numVerts;
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, vertexArrayNumBytes, verts.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TheRenderer::DrawVBO(int numVerts, PrimitiveType type, int &vboID, bool isTextured) {
	glBindBuffer(GL_ARRAY_BUFFER, vboID);

	if (isTextured) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, sizeof(TexturedVertex), (const GLvoid*)offsetof(TexturedVertex, m_position));
		glColorPointer(4, GL_FLOAT, sizeof(TexturedVertex), (const GLvoid*)offsetof(TexturedVertex, m_color));
		glTexCoordPointer(2, GL_FLOAT, sizeof(TexturedVertex), (const GLvoid*)offsetof(TexturedVertex, m_texCoords));

		glDrawArrays(GetGLEnumForPrimitiveType(type), 0, numVerts);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	else {
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, m_position));
		glColorPointer(4, GL_FLOAT, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, m_color));


		glDrawArrays(GetGLEnumForPrimitiveType(type), 0, numVerts);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TheRenderer::ClearVBOData(int vboID) {
	GLuint id = vboID;
	glDeleteBuffers(1, &id);
}

//----------------------------------VIEW MATRIX TRANSLATIONS--------------------------------------
void TheRenderer::LoadIdentity() {
	glLoadIdentity();
}
void TheRenderer::SetOrtho2D(const Vector2& mins, const Vector2& maxs) {
	glLoadIdentity();
	glOrtho(mins.x, maxs.x, mins.y, maxs.y, 0.f, 1.f);
}
void TheRenderer::SetPerspective(float fieldOfViewYAxis, float aspect, float nearClippingPlane, float farClippingPlane) {
	glLoadIdentity();
	gluPerspective(fieldOfViewYAxis, aspect, nearClippingPlane, farClippingPlane);
}

void TheRenderer::TranslateViewMatrix(const Vector2& offset) {
	glTranslatef(offset.x, offset.y, 0.f);
}
void TheRenderer::TranslateViewMatrix(const Vector3& offset) {
	glTranslatef(offset.x, offset.y, offset.z);
}
void TheRenderer::RotateViewMatrix(float angle) {
	glRotatef(ToDegrees(angle), 0.f, 0.f, 1.f);
}
void TheRenderer::RotateViewMatrix(float angle, const Vector3& axisOfRotation) {
	glRotatef(angle, axisOfRotation.x, axisOfRotation.y, axisOfRotation.z);
}
void TheRenderer::PushViewMatrix() {
	glPushMatrix();
}
void TheRenderer::PopViewMatrix() {
	glPopMatrix();
}
void TheRenderer::ScaleViewMatrix2D(Vector2 scale) {
	glScalef(scale.x, scale.y, 0.f);
}
void TheRenderer::ScaleViewMatrix3D(Vector3 scale) {
	glScalef(scale.x, scale.y, scale.z);
}

//----------------------------------DRAWING / COLORING FUNCS--------------------------------------
void TheRenderer::MotionBlur() {
	glColor4f(0.f, 0.f, 0.f, .9f); //-Black with high alpha
	glRectf(0.f, 0.f, 16.f, 9.f); //over screen to get it from looking "framey"
	glAccum(GL_MULT, 0.45f); //-Adds the actual motion blur
}
void TheRenderer::ClearColor(const RGBA& color) {
	glClearColor(color.r(), color.g(), color.b(), color.a());
}
void TheRenderer::SetColor(const RGBA& color) {
	m_colorMode = color;
}

//DEBUG DRAWING
void TheRenderer::AddDebugPoint(const Vector3& position, const RGBA& color) {
	m_debugPoints.push_back(Vertex(position, color));
}
void TheRenderer::DrawDebugPoints(float pointSize, bool enableDepthTesting) {
	if (enableDepthTesting)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	glPointSize(pointSize);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &m_debugPoints[0].m_position);
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), &m_debugPoints[0].m_color);

	glDrawArrays(GL_POINTS, 0, m_debugPoints.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}
void TheRenderer::ClearDebugPoints() {
	m_debugPoints.clear();
}

GLenum TheRenderer::GetGLEnumForPrimitiveType(PrimitiveType type) {

	switch (type) {
	case PRIMITIVE_POINTS:
		return GL_POINTS;
	case PRIMITIVE_LINES:
		return GL_LINES;
	case PRIMITIVE_LINE_LOOP:
		return GL_LINE_LOOP;
	case PRIMITIVE_TRIANGLES:
		return GL_TRIANGLES;
	case PRIMITIVE_QUADS:
		return GL_QUADS;
	default:
		return GL_LINES;
	}
}

void TheRenderer::EnableGLAlphaFunc(float amount) {
	glAlphaFunc(GL_GREATER, amount);
}
void TheRenderer::DisableGLAlphaFunc() {

}