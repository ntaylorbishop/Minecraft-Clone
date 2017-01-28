#pragma once

#include <Windows.h>

const int NUM_KEYS = 256;

class Input;
extern Input* g_theInput;

class Input {
public:
	//STRUCTORS
	Input();

	bool hasQuit();
	void quitting(bool isNowQuitting);

	bool getKey(unsigned char asKey);
	bool getKeyDown(unsigned char asKey);
	void setKeyDownStatus(unsigned char asKey, bool isNowDown);

private:
	bool m_isKeyDown[NUM_KEYS];
	bool quit;
};