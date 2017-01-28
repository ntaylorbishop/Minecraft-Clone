#pragma once

#include <string>
#include "Engine/StringUtils.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Game/TheWorld.hpp"
#include "Game/Raycast3D.hpp"

class TheCamera;
extern TheCamera* g_theCamera;

enum CameraMode {
	CAMERA_MODE_FIRST_PERSON = 0,
	CAMERA_MODE_THIRD_PERSON,
	CAMERA_MODE_SPECTATOR
};

using namespace std;

class TheCamera {
public:
	//STRUCTORS
	TheCamera(CameraMode mode = CAMERA_MODE_FIRST_PERSON);
	~TheCamera();

	//UPDATE
	void Update();

	WorldPosition GetFurthestNonObstructedPosition(const WorldPosition& eyeOfPlayer, const WorldPosition& displacement);

	//ACCESSORS
	CameraMode GetCameraMode() const;
	string GetCameraModeStringified();
	void SetCameraMode(CameraMode mode);

	Vector3 GetForwardXYZ() const;


public:
	Vector3 m_position;
	float m_rollAboutX;
	float m_pitchAboutYDeg;
	float m_yawAboutZ;

	Vector3 m_cameraEyeDisplacement;

	CameraMode m_cameraMode;
};