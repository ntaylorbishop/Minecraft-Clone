#include "Game/WeatherSheet.hpp"
#include "Game/ThePlayer.hpp"

WeatherSheet::WeatherSheet() 
     : m_position(WorldPosition(0.f, 0.f, 0.f))
     , m_type(WEATHER_SNOW)
	 , m_rainTexture(nullptr)
	 , m_snowTexture(nullptr)
	 , m_age(0.f)
	 , m_speedVariance(0.f)
	 , m_minHeight(0.f)
	 , m_xyVariance(0.f)
{
	InitializeSheet();
}
WeatherSheet::WeatherSheet(const WorldPosition& loc, const SheetType& type, float minHeight)
    : m_position(loc)
	, m_type(type)
    , m_rainTexture(nullptr)
    , m_snowTexture(nullptr)
	, m_age(0.f)
	, m_speedVariance(0.f)
	, m_minHeight(minHeight)
	, m_xyVariance(0.f)
{
	InitializeSheet();
}
WeatherSheet::~WeatherSheet() { }

void WeatherSheet::InitializeSheet() {
	m_rainTexture = Texture::CreateOrGetTexture(m_rainTextureToUse);
	m_snowTexture = Texture::CreateOrGetTexture(m_snowTextureToUse);

	if (m_type == WEATHER_RAIN) {
		m_zVariance = Randf(m_minHeight, BLOCKS_WIDE_Z);
	}
	else if (m_type == WEATHER_SNOW) {
		m_zVariance = Randf(m_minHeight, m_minHeight + 10.f);
	}

	m_speedVariance = Randf(-SHEET_SPEED_VARIANCE, SHEET_SPEED_VARIANCE);
	m_position += Vector3(0.f, 0.f, m_zVariance);

	m_xyVariance = Randf(-0.1f, 0.1f);
}


void WeatherSheet::Update(float deltaSeconds) {
	m_age += deltaSeconds;

	if (m_type == WEATHER_SNOW)
		m_position += Vector3(m_xyVariance, m_xyVariance, m_speedVariance + SHEET_SPEED_SNOW) * deltaSeconds;
	else if (m_type == WEATHER_RAIN)
		m_position += Vector3(0.f, 0.f, m_speedVariance + SHEET_SPEED_RAIN) * deltaSeconds;

	BlockInfo currBlock = TheWorld::GetBlockInfoForWorldPosition(m_position);
	if (currBlock.GetBlock() && BlockDefinition::IsSolid(currBlock.GetBlock()->GetBlockType()) || m_position.z <= m_minHeight - 1.f) {
		if (m_type == WEATHER_RAIN)
			m_position.z = m_zVariance + 30.f;
		else if (m_type == WEATHER_SNOW)
			m_position.z = m_zVariance + 1.f;
	}

	ComputeVertices();
}
void WeatherSheet::ComputeVertices() {
	m_verts.clear();

	Vector2 playerPos2D = Vector2(g_thePlayer->m_position.x, g_thePlayer->m_position.y);
	Vector2 sheetPos2D = Vector2(m_position.x, m_position.y);
	float distBetweenSheetAndPlayer = DistanceBetweenPoints(playerPos2D, sheetPos2D);

	if (distBetweenSheetAndPlayer < WEATHER_RADIUS_MAX && distBetweenSheetAndPlayer > WEATHER_RADIUS_MIN) {
		Vector3 topLeft1 =     Vector3(0.f, 0.f, 0.f);
		Vector3 topRight1 =    Vector3(0.f, SHEET_WIDTH, 0.f);
		Vector3 bottomRight1 = Vector3(0.f, SHEET_WIDTH, SHEET_HEIGHT);
		Vector3 bottomLeft1 =  Vector3(0.f, 0.f, SHEET_HEIGHT);

		Vector3 topLeft2 =     Vector3(0.f, 0.f, 0.f);
		Vector3 topRight2 =    Vector3(SHEET_WIDTH, 0.f, 0.f);
		Vector3 bottomRight2 = Vector3(SHEET_WIDTH, 0.f, SHEET_HEIGHT);
		Vector3 bottomLeft2 =  Vector3(0.f, 0.f, SHEET_HEIGHT);

		//SHEET 1
		Vector3 startPos1 = m_position + topLeft1;
		Vector3 startPos2 = m_position + topRight1;
		Vector3 startPos3 = m_position + bottomRight1;
		Vector3 startPos4 = m_position + bottomLeft1;

		//First set of sheets
		m_verts.push_back(TexturedVertex(startPos1, RGBA::WHITE, Vector2::TEXCOORD_TOP_LEFT));
		m_verts.push_back(TexturedVertex(startPos2, RGBA::WHITE, Vector2::TEXCOORD_TOP_RIGHT));
		m_verts.push_back(TexturedVertex(startPos3, RGBA::WHITE, Vector2::TEXCOORD_BOTTOM_RIGHT));
		m_verts.push_back(TexturedVertex(startPos4, RGBA::WHITE, Vector2::TEXCOORD_BOTTOM_LEFT));

		//SHEET 2
		startPos1 = m_position + topLeft2;
		startPos2 = m_position + topRight2;
		startPos3 = m_position + bottomRight2;
		startPos4 = m_position + bottomLeft2;

		//Second set of sheets
		m_verts.push_back(TexturedVertex(startPos1, RGBA::WHITE, Vector2::TEXCOORD_TOP_LEFT));
		m_verts.push_back(TexturedVertex(startPos2, RGBA::WHITE, Vector2::TEXCOORD_TOP_RIGHT));
		m_verts.push_back(TexturedVertex(startPos3, RGBA::WHITE, Vector2::TEXCOORD_BOTTOM_RIGHT));
		m_verts.push_back(TexturedVertex(startPos4, RGBA::WHITE, Vector2::TEXCOORD_BOTTOM_LEFT));
	}
}

float WeatherSheet::GetDistanceToSheet() {
	Vector2 playerPos2D = Vector2(g_thePlayer->m_position.x, g_thePlayer->m_position.y);
	Vector2 sheetPos2D = Vector2(m_position.x, m_position.y);

	return DistanceBetweenPoints(playerPos2D, sheetPos2D);
}

const vector<TexturedVertex>* WeatherSheet::GetVerts() const {
	return &m_verts;
}
Texture* WeatherSheet::GetRainTexture() const {
	return m_rainTexture;
}
Texture* WeatherSheet::GetSnowTexture() const {
	return m_snowTexture;
}
SheetType WeatherSheet::GetType() const {
	return m_type;
}