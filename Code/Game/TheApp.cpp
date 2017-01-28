#include "Game/TheApp.hpp"

TheApp* g_theApp = nullptr;

TheApp::TheApp(ScreenCoords screenDimensions) 
    : m_screenDimensions(screenDimensions)
{ }
TheApp::~TheApp() {

}

bool TheApp::hasFocus() {
	return m_windowInFocus;
}
void TheApp::onGainedFocus() {
	m_windowInFocus = true;
}
void TheApp::onLostFocus() {
	m_windowInFocus = false;
}

ScreenCoords TheApp::getScreenDimensions() const {
	return m_screenDimensions;
}