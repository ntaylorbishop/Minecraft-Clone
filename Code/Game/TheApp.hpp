#pragma once

#include "Engine/EngineCommon.hpp"
#include "Engine/Math/IntVector2.hpp"

class TheApp;
extern TheApp* g_theApp;

class TheApp {
public:
	TheApp(ScreenCoords screenDimensions);
	~TheApp();

	bool hasFocus();
	void onGainedFocus();
	void onLostFocus();

	ScreenCoords getScreenDimensions() const;
private:
	ScreenCoords m_screenDimensions;
	bool m_windowInFocus;
};