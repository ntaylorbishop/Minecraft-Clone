#include "Game/ThePlayer.hpp"
#include "Game/TheCamera.hpp"

TheCamera* g_theCamera = nullptr;


//STRUCTORS-------------------------------------------------------------------------------------------------------------------------------------
TheCamera::TheCamera(CameraMode mode)
    : m_position()
    , m_rollAboutX(0.f)
	, m_pitchAboutYDeg(0.f)
	, m_yawAboutZ(0.f)
	, m_cameraMode(mode)
	, m_cameraEyeDisplacement()
{ }

TheCamera::~TheCamera() { }

//UPDATE-------------------------------------------------------------------------------------------------------------------------------------
void TheCamera::Update() {
	m_cameraEyeDisplacement = g_thePlayer->m_position + Vector3(0.f, 0.f, 0.69f);

	if (m_cameraMode == CAMERA_MODE_FIRST_PERSON) {
		m_pitchAboutYDeg = g_thePlayer->m_pitchAboutYDeg;
		m_rollAboutX = g_thePlayer->m_rollAboutX;
		m_yawAboutZ = g_thePlayer->m_yawAboutZ;

		m_position = m_cameraEyeDisplacement;
	}
	else if (m_cameraMode == CAMERA_MODE_THIRD_PERSON) {
		m_pitchAboutYDeg = g_thePlayer->m_pitchAboutYDeg;
		m_rollAboutX = g_thePlayer->m_rollAboutX;
		m_yawAboutZ = g_thePlayer->m_yawAboutZ;

		m_position = GetFurthestNonObstructedPosition(m_cameraEyeDisplacement, -5.f * g_thePlayer->GetForwardXYZ());
	}
	else if (m_cameraMode == CAMERA_MODE_SPECTATOR) {
		m_yawAboutZ = 45.f;
		m_pitchAboutYDeg = 20.f;
		m_rollAboutX = 0.f;

		m_position = GetFurthestNonObstructedPosition(m_cameraEyeDisplacement, Vector3(-9.f, -6.f, 5.f));
	}
}

WorldPosition TheCamera::GetFurthestNonObstructedPosition(const WorldPosition& eyeOfPlayer, const WorldPosition& displacement) {
	WorldPosition start = eyeOfPlayer;
	WorldPosition end = eyeOfPlayer + displacement;

	RaycastResult result;
	if (Raycast3DToPoint(start, end, &result)) {
		return result.m_prevImpactPos;
	}
	else {
		return eyeOfPlayer + displacement;
	}
}

//ACCESSORS-------------------------------------------------------------------------------------------------------------------------------------
CameraMode TheCamera::GetCameraMode() const {
	return m_cameraMode;
}
string TheCamera::GetCameraModeStringified() {
	if (m_cameraMode == CAMERA_MODE_FIRST_PERSON)
		return "First Person";
	else if (m_cameraMode == CAMERA_MODE_THIRD_PERSON)
		return "Third Person";
	else if (m_cameraMode == CAMERA_MODE_SPECTATOR)
		return "Spectator";
	else
		return "ERROR";
}
void TheCamera::SetCameraMode(CameraMode mode) {
	m_cameraMode = mode;
}

Vector3 TheCamera::GetForwardXYZ() const {
	return Vector3(CosDegrees(m_yawAboutZ) * CosDegrees(m_pitchAboutYDeg), SinDegrees(m_yawAboutZ) * CosDegrees(m_pitchAboutYDeg), SinDegrees(-m_pitchAboutYDeg));
}