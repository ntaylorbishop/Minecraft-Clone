#pragma once

#include <Windows.h>
#include "Engine/EngineCommon.hpp"
#include "Engine/Math/IntVector2.hpp"

const int NUM_KEYS = 256;

class InputSystem;
extern InputSystem* g_theInputSystem;

enum MouseButton {
	MOUSE_LEFT = 0,
	MOUSE_MIDDLE,
	MOUSE_RIGHT
};

class InputSystem {
public:
	//STRUCTORS
	InputSystem();

	bool HasQuit();
	void Quitting(bool isNowQuitting);

	bool GetKey(unsigned char asKey);
	bool GetKeyDown(unsigned char asKey);
	void SetKeyDownStatus(unsigned char asKey, bool isNowDown);

	bool GetMouseBtn(MouseButton btn);
	bool GetMouseBtnDown(MouseButton btn);
	void SetMouseBtnDown(MouseButton btn, bool isDown);

	void SetMouseWheelDelta(short wheelDelta);
	bool GetMouseWheelUp();
	bool GetMouseWheelDown();

	void SetCursorPosition(ScreenCoords pos);
	ScreenCoords GetCursorPosition();
	void HideMouseCursor();
	void ShowMouseCursor();

private:
	bool m_isKeyDown[NUM_KEYS];
	bool quit;
	bool m_rightMouseDown;
	bool m_leftMouseDown;
	bool m_middleMouseDown;

	int m_mouseWheelUpDelta;
	int m_mouseWheelDownDelta;
};

//TODO: Fix GetKeyDown / GetMouseBtnDown to work with frame count instead of the janky way its set up now