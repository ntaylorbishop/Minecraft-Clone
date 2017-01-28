#include "Engine/InputSystem.hpp"

InputSystem* g_theInputSystem = false;

InputSystem::InputSystem() {
	for (int i = 0; i < 255; i++)
		m_isKeyDown[i] = false;
	quit = false;

	m_leftMouseDown = false;
	m_middleMouseDown = false;
	m_rightMouseDown = false;
}

bool InputSystem::HasQuit() {
	return quit;
}

bool InputSystem::GetKey(unsigned char asKey) {
	return m_isKeyDown[asKey];
}
bool InputSystem::GetKeyDown(unsigned char asKey) {
	bool b = m_isKeyDown[asKey];
	if (b) {
		m_isKeyDown[asKey];
		m_isKeyDown[asKey] = false;
	}
	return b;
}
void InputSystem::SetKeyDownStatus(unsigned char asKey, bool isNowDown) {
	m_isKeyDown[asKey] = isNowDown;
}

bool InputSystem::GetMouseBtn(MouseButton btn) {
	if (btn == MOUSE_LEFT)
		return m_leftMouseDown;
	else if (btn == MOUSE_MIDDLE)
		return m_middleMouseDown;
	else if (btn == MOUSE_RIGHT)
		return m_rightMouseDown;
	else {
		SystemDialogue_Okay("Tried to get an invalid mouse key", "A function tried to see if an invalid mouse button was down.", SEVERITY_WARNING);
		return false;
	}
}
bool InputSystem::GetMouseBtnDown(MouseButton btn) {
	if (btn == MOUSE_LEFT) {
		bool b = m_leftMouseDown;
		m_leftMouseDown = false;
		return b;
	}
	else if (btn == MOUSE_MIDDLE) {
		bool b = m_middleMouseDown;
		m_middleMouseDown = false;
		return b;
	}
	else if (btn == MOUSE_RIGHT) {
		bool b = m_rightMouseDown;
		m_rightMouseDown = false;
		return b;
	}
	else {
		SystemDialogue_Okay("Tried to get an invalid mouse key", "A function tried to see if an invalid mouse button was down.", SEVERITY_WARNING);
		return false;
	}
}
void InputSystem::SetMouseBtnDown(MouseButton btn, bool isDown) {
	if (btn == MOUSE_LEFT)
		m_leftMouseDown = isDown;
	else if (btn == MOUSE_MIDDLE)
		m_middleMouseDown = isDown;
	else if (btn == MOUSE_RIGHT)
		m_rightMouseDown = isDown;
	else {
		SystemDialogue_Okay("Tried to get an invalid mouse key", "A function tried to see if an invalid mouse button was down.", SEVERITY_WARNING);
	}
}

void InputSystem::SetMouseWheelDelta(short wheelDelta) {
	m_mouseWheelUpDelta = (int)wheelDelta;
	m_mouseWheelDownDelta = (int)wheelDelta;
}
bool InputSystem::GetMouseWheelUp() {
	int wheel = m_mouseWheelUpDelta;
	m_mouseWheelUpDelta = 0;
	return wheel > 0;
}
bool InputSystem::GetMouseWheelDown() {
	int wheel = m_mouseWheelDownDelta;
	m_mouseWheelDownDelta = 0;
	return wheel < 0;
}

void InputSystem::Quitting(bool isNowQuitting) {
	quit = isNowQuitting;
}

void InputSystem::SetCursorPosition(ScreenCoords pos) {
	SetCursorPos(pos.x, pos.y);
}
ScreenCoords InputSystem::GetCursorPosition() {
	POINT cursorPos;
	GetCursorPos(&cursorPos);

	return ScreenCoords((int)cursorPos.x, (int)cursorPos.y);
}

void InputSystem::HideMouseCursor() {
	ShowCursor(false);
}
void InputSystem::ShowMouseCursor() {
	ShowCursor(true);
}