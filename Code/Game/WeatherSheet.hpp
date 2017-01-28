#pragma once

#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Game/GameCommon.hpp"

enum SheetType {
	WEATHER_SNOW = 0,
	WEATHER_RAIN
};

const float SHEET_SPEED_SNOW = -0.1f;
const float SHEET_SPEED_RAIN = -50.f;
const float SHEET_SPEED_VARIANCE = 0.001f;
const float WEATHER_RADIUS_MAX = 20.f;
const float WEATHER_RADIUS_MIN = 2.f;

const float SHEET_WIDTH = 1.f;
const float SHEET_HEIGHT = 64.f;

class WeatherSheet {
public:
	//STRUCTORS
	WeatherSheet();
	WeatherSheet(const WorldPosition& loc, const SheetType& type, float minHeight);
	~WeatherSheet();

	//INITIALIZATION
	void InitializeSheet();

	//UPDATE / RENDER
	void Update(float deltaSeconds);
	void ComputeVertices();

	//GETTERS SETTERS
	float GetDistanceToSheet();
	const vector<TexturedVertex>* GetVerts() const;
	Texture* GetRainTexture() const;
	Texture* GetSnowTexture() const;
	SheetType GetType() const;

	//OPERATOR OVERLOADS
	bool operator==(const WeatherSheet& other) const {
		if (this->m_position.x == other.m_position.x && this->m_position.y == other.m_position.y && this->m_position.z == other.m_position.z)
			return true;
		else
			return false;
	}
	bool operator!=(const WeatherSheet& other) const {
		return !(*this == other);
	}

private:
	char* m_rainTextureToUse = "Data/Images/rain.png";
	char* m_snowTextureToUse = "Data/Images/snow.png";
	Texture* m_rainTexture;
	Texture* m_snowTexture;

	WorldPosition m_position;
	SheetType m_type;

	float m_zVariance;
	float m_xyVariance;
	float m_age;
	float m_speedVariance;
	float m_minHeight;

	vector<TexturedVertex> m_verts;
};