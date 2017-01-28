#include "Game/Input.hpp"

#define STATIC

Input* g_theInput = false;

Input::Input() {
	quit = false;
}

bool Input::hasQuit() {
	return quit;
}

bool Input::getKey(unsigned char asKey) {
	return m_isKeyDown[asKey];
}
bool Input::getKeyDown(unsigned char asKey) {
	bool b = m_isKeyDown[asKey];
	if (b)
		m_isKeyDown[asKey];
	return b;
}

void Input::setKeyDownStatus(unsigned char asKey, bool isNowDown) {
	m_isKeyDown[asKey] = isNowDown;
}
void Input::quitting(bool isNowQuitting) {
	quit = isNowQuitting;
}