#include "XboxController.hpp"

XboxController::XboxController() {
	this->controllerNumber = 0;

	leftTrigger = 0;
	rightTrigger = 0;
	connected = false;

	lThumbPolar = PolarCoords(0.f, 0.f);
	rThumbPolar = PolarCoords(0.f, 0.f);

	frameNumber = 0;
	for (int i = 0; i < 14; i++)
		button_frameNumber[i] = -1;
}
XboxController::XboxController(int controllerNumber) {
	this->controllerNumber = controllerNumber;

	leftTrigger = 0;
	rightTrigger = 0;
	connected = false;

	lThumbPolar = PolarCoords(0.f, 0.f);
	rThumbPolar = PolarCoords(0.f, 0.f);

	frameNumber = 0;
	for (int i = 0; i < 14; i++)
		button_frameNumber[i] = -1;
}

void XboxController::Update() {
	XINPUT_STATE xboxControllerState;
	memset(&xboxControllerState, 0, sizeof(xboxControllerState)); 
	DWORD errorStatus = XInputGetState(controllerNumber, &xboxControllerState);

	if (errorStatus == ERROR_SUCCESS) {
		if (frameNumber == 0) {
			prevButtons = xboxControllerState.Gamepad.wButtons;
			buttons = xboxControllerState.Gamepad.wButtons;
		}
		else {
			prevButtons = buttons;
			buttons = xboxControllerState.Gamepad.wButtons;
		}


		frameNumber++;

		//Check to see if a button went down this frame
		if (((buttons & 0x0001)) != 0 && prevButtons != buttons) button_frameNumber[0] = frameNumber;
		if (((buttons & 0x0002)) != 0 && prevButtons != buttons) button_frameNumber[1] = frameNumber;
		if (((buttons & 0x0004)) != 0 && prevButtons != buttons) button_frameNumber[2] = frameNumber;
		if (((buttons & 0x0008)) != 0 && prevButtons != buttons) button_frameNumber[3] = frameNumber;
		if (((buttons & 0x0010)) != 0 && prevButtons != buttons) button_frameNumber[4] = frameNumber;
		if (((buttons & 0x0020)) != 0 && prevButtons != buttons) button_frameNumber[5] = frameNumber;
		if (((buttons & 0x0040)) != 0 && prevButtons != buttons) button_frameNumber[6] = frameNumber;
		if (((buttons & 0x0080)) != 0 && prevButtons != buttons) button_frameNumber[7] = frameNumber;
		if (((buttons & 0x0100)) != 0 && prevButtons != buttons) button_frameNumber[8] = frameNumber;
		if (((buttons & 0x0200)) != 0 && prevButtons != buttons) button_frameNumber[9] = frameNumber;
		if (((buttons & 0x1000)) != 0 && prevButtons != buttons) button_frameNumber[10] = frameNumber;
		if (((buttons & 0x2000)) != 0 && prevButtons != buttons) button_frameNumber[11] = frameNumber;
		if (((buttons & 0x4000)) != 0 && prevButtons != buttons) button_frameNumber[12] = frameNumber;
		if (((buttons & 0x8000)) != 0 && prevButtons != buttons) button_frameNumber[13] = frameNumber;

		lThumb.x = (float)xboxControllerState.Gamepad.sThumbLX;
		lThumb.y = (float)xboxControllerState.Gamepad.sThumbLY;
		rThumb.x = xboxControllerState.Gamepad.sThumbRX;
		rThumb.y = xboxControllerState.Gamepad.sThumbRY;
		leftTrigger = (int)xboxControllerState.Gamepad.bLeftTrigger;
		rightTrigger = (int)xboxControllerState.Gamepad.bRightTrigger;

		updateLeftStick();
		updateRightStick();

		connected = true;
	}
	else if (errorStatus == ERROR_DEVICE_NOT_CONNECTED) {
		connected = false;
	}
}
void XboxController::updateLeftStick() {
	float xNorm = (float)lThumb.x / 32000.f;
	float yNorm = (float)lThumb.y / 32000.f;

	Vector2 leftThumbNorm = Vector2(xNorm, yNorm);
	float radius = leftThumbNorm.length();
	float angle = atan2(yNorm, xNorm);

	float radiusMapped = RangeMap(radius, .3f, .976f, 0.f, 1.f);
	radiusMapped = ClampFloat(radiusMapped, 0.f, 1.f);

	lThumbPolar = PolarCoords(radiusMapped, angle);
}
void XboxController::updateRightStick() {
	float xNorm = (float)rThumb.x / 32000.f;
	float yNorm = (float)rThumb.y / 32000.f;

	Vector2 leftThumbNorm = Vector2(xNorm, yNorm);
	float radius = leftThumbNorm.length();
	float angle = atan2(yNorm, xNorm);

	float radiusMapped = RangeMap(radius, .3f, .976f, 0.f, 1.f);
	radiusMapped = ClampFloat(radiusMapped, 0.f, 1.f);

	rThumbPolar = PolarCoords(radiusMapped, angle);
}

bool XboxController::getButton(button btn) const {
	if      (btn == XB_DPAD_UP)     return ((buttons & 0x0001) != 0);
	else if (btn == XB_DPAD_DOWN)   return ((buttons & 0x0002) != 0);
	else if (btn == XB_DPAD_LEFT)   return ((buttons & 0x0004) != 0);
	else if (btn == XB_DPAD_RIGHT)  return ((buttons & 0x0008) != 0);
	else if (btn == XB_START)       return ((buttons & 0x0010) != 0);
	else if (btn == XB_BACK)        return ((buttons & 0x0020) != 0);
	else if (btn == XB_LEFT_THUMB)  return ((buttons & 0x0040) != 0);
	else if (btn == XB_RIGHT_THUMB) return ((buttons & 0x0080) != 0);
	else if (btn == XB_LB)          return ((buttons & 0x0100) != 0);
	else if (btn == XB_RB)          return ((buttons & 0x0200) != 0);
	else if (btn == XB_A)           return ((buttons & 0x1000) != 0);
	else if (btn == XB_B)           return ((buttons & 0x2000) != 0);
	else if (btn == XB_X)           return ((buttons & 0x4000) != 0);
	else if (btn == XB_Y)           return ((buttons & 0x8000) != 0);
	else
		return false;
}
bool XboxController::getButtonDown(button btn) const {
	if      (btn == XB_DPAD_UP)     return (button_frameNumber[0] == frameNumber);
	else if (btn == XB_DPAD_DOWN)   return (button_frameNumber[1] == frameNumber);
	else if (btn == XB_DPAD_LEFT)   return (button_frameNumber[2] == frameNumber);
	else if (btn == XB_DPAD_RIGHT)  return (button_frameNumber[3] == frameNumber);
	else if (btn == XB_START)       return (button_frameNumber[4] == frameNumber);
	else if (btn == XB_BACK)        return (button_frameNumber[5] == frameNumber);
	else if (btn == XB_LEFT_THUMB)  return (button_frameNumber[6] == frameNumber);
	else if (btn == XB_RIGHT_THUMB) return (button_frameNumber[7] == frameNumber);
	else if (btn == XB_LB)          return (button_frameNumber[8] == frameNumber);
	else if (btn == XB_RB)          return (button_frameNumber[9] == frameNumber);
	else if (btn == XB_A)           return (button_frameNumber[10] == frameNumber);
	else if (btn == XB_B)           return (button_frameNumber[11] == frameNumber);
	else if (btn == XB_X)           return (button_frameNumber[12] == frameNumber);
	else if (btn == XB_Y)           return (button_frameNumber[13] == frameNumber);
	else
		return false;
}

PolarCoords XboxController::getLeftStickPositionPolar() {
	return lThumbPolar;
}
PolarCoords XboxController::getRightStickPositionPolar() {
	return rThumbPolar;
}
Vector2 XboxController::getLeftStickPosition() {
	return Vector2(lThumbPolar.radius * cos(lThumbPolar.theta), lThumbPolar.radius * sin(rThumbPolar.theta));
}
Vector2 XboxController::getRightStickPosition() {
	return Vector2(rThumbPolar.radius * cos(rThumbPolar.theta), rThumbPolar.radius * sin(rThumbPolar.theta));
}

float XboxController::getLeftTriggerValue() const {
	float lTriggerCast = (float)leftTrigger;
	return RangeMap(lTriggerCast, 0.f, 255.f, 0.f, 1.f);
}
float XboxController::getRightTriggerValue() const {
	float rTriggerCast = (float)rightTrigger;
	return RangeMap(rTriggerCast, 0.f, 255.f, 0.f, 1.f);
}

bool XboxController::isConnected() {
	return connected;
}