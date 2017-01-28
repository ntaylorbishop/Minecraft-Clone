#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <gl/gl.h>
#include <math.h>
#include <cassert>
#include <crtdbg.h>
#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library

#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/InputSystem.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Time.hpp"
#include "Game/TheGame.hpp"
#include "Game/TheApp.hpp"

#define UNUSED(x) (void)(x);

//--------------Window Creation----------------------------
const int OFFSET_FROM_WINDOWS_DESKTOP = 10;
const int WINDOW_PHYSICAL_WIDTH = 1600;
const int WINDOW_PHYSICAL_HEIGHT = 900;
const double VIEW_LEFT = 0.0;
const double VIEW_RIGHT = 1600.0;
const double VIEW_BOTTOM = 0.0;
const double VIEW_TOP = VIEW_RIGHT * static_cast< double >(WINDOW_PHYSICAL_HEIGHT) / static_cast< double >(WINDOW_PHYSICAL_WIDTH);

HWND g_hWnd = nullptr;
HDC g_displayDeviceContext = nullptr;
HGLRC g_openGLRenderingContext = nullptr;
const char* APP_NAME = "SimpleMiner"; 

LRESULT CALLBACK WindowsMessageHandlingProcedure(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam) {
	unsigned char asKey = (unsigned char)wParam;
	switch (wmMessageCode) {
	case WM_CLOSE:
	case WM_DESTROY:
	case WM_QUIT:
		g_theInputSystem->Quitting(true);
		return 0;
	case WM_KEYDOWN:
		g_theInputSystem->SetKeyDownStatus(asKey, true);

		if (asKey == VK_ESCAPE) {
			g_theInputSystem->Quitting(true);
			return 0;
		}
		break;

	case WM_KEYUP:
		g_theInputSystem->SetKeyDownStatus(asKey, false);
		break;
	case WM_SETFOCUS:
		g_theApp->onGainedFocus();
		break;
	case WM_KILLFOCUS:
		g_theApp->onLostFocus();
		break;
	case WM_LBUTTONDOWN:
		g_theInputSystem->SetMouseBtnDown(MOUSE_LEFT, true);
		break;
	case WM_LBUTTONUP:
		g_theInputSystem->SetMouseBtnDown(MOUSE_LEFT, false);
		break;
	case WM_MBUTTONDOWN:
		g_theInputSystem->SetMouseBtnDown(MOUSE_MIDDLE, true);
		break;
	case WM_MBUTTONUP:
		g_theInputSystem->SetMouseBtnDown(MOUSE_MIDDLE, false);
		break;
	case WM_RBUTTONDOWN:
		g_theInputSystem->SetMouseBtnDown(MOUSE_RIGHT, true);
		break;
	case WM_RBUTTONUP:
		g_theInputSystem->SetMouseBtnDown(MOUSE_RIGHT, false);
		break;
	case WM_MOUSEWHEEL:
		g_theInputSystem->SetMouseWheelDelta(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	}

	return DefWindowProc(windowHandle, wmMessageCode, wParam, lParam);
}

void CreateOpenGLWindow(HINSTANCE applicationInstanceHandle) {
	// Define a window class
	WNDCLASSEX windowClassDescription;
	memset(&windowClassDescription, 0, sizeof(windowClassDescription));
	windowClassDescription.cbSize = sizeof(windowClassDescription);
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast< WNDPROC >(WindowsMessageHandlingProcedure); // Assign a win32 message-handling function
	windowClassDescription.hInstance = GetModuleHandle(NULL);
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT("Simple Window Class");
	RegisterClassEx(&windowClassDescription);

// 	const DWORD windowStyleFlags = WS_POPUP;
// 	const DWORD windowStyleExFlags = WS_EX_APPWINDOW | WS_EX_TOPMOST;

	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect(desktopWindowHandle, &desktopRect);

	RECT windowRect = { OFFSET_FROM_WINDOWS_DESKTOP, OFFSET_FROM_WINDOWS_DESKTOP, OFFSET_FROM_WINDOWS_DESKTOP + WINDOW_PHYSICAL_WIDTH, OFFSET_FROM_WINDOWS_DESKTOP + WINDOW_PHYSICAL_HEIGHT };
	AdjustWindowRectEx(&windowRect, windowStyleFlags, FALSE, windowStyleExFlags);

	WCHAR windowTitle[1024];
	MultiByteToWideChar(GetACP(), 0, APP_NAME, -1, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
	g_hWnd = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		applicationInstanceHandle,
		NULL
	);

	ShowWindow(g_hWnd, SW_SHOW);
	SetForegroundWindow(g_hWnd);
	SetFocus(g_hWnd);

	g_displayDeviceContext = GetDC(g_hWnd);

	HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
	SetCursor(cursor);

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	memset(&pixelFormatDescriptor, 0, sizeof(pixelFormatDescriptor));
	pixelFormatDescriptor.nSize = sizeof(pixelFormatDescriptor);
	pixelFormatDescriptor.nVersion = 1;
	pixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDescriptor.cColorBits = 24;
	pixelFormatDescriptor.cDepthBits = 24;
	pixelFormatDescriptor.cAccumBits = 0;
	pixelFormatDescriptor.cStencilBits = 8;

	int pixelFormatCode = ChoosePixelFormat(g_displayDeviceContext, &pixelFormatDescriptor);
	SetPixelFormat(g_displayDeviceContext, pixelFormatCode, &pixelFormatDescriptor);
	g_openGLRenderingContext = wglCreateContext(g_displayDeviceContext);
	wglMakeCurrent(g_displayDeviceContext, g_openGLRenderingContext);

	//Set some standard 2D settings up. TheGame will change it if it wants
	//Renderer::SetOrtho2D(Vector2(VIEW_LEFT, VIEW_BOTTOM), Vector2(VIEW_RIGHT, VIEW_TOP));
	g_theRenderer->Enable(GL_BLEND);
	g_theRenderer->BlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	g_theRenderer->LineWidth(2.f);
	g_theRenderer->Enable(GL_LINE_SMOOTH);
	g_theRenderer->EnableGLAlphaFunc(0.3f);
}

//-----------Main game Loop--------------------------------
void RunMessagePump() {
	MSG queuedMessage;
	for (;;)
	{
		const BOOL wasMessagePresent = PeekMessage(&queuedMessage, NULL, 0, 0, PM_REMOVE);
		if (!wasMessagePresent)
		{
			break;
		}

		TranslateMessage(&queuedMessage);
		DispatchMessage(&queuedMessage);
	}
}
void Update() {
	static double s_timeLastFrameBegan = GetCurrentTimeSeconds();

	double timeThisFrameBegan = GetCurrentTimeSeconds();
	float deltaSeconds = static_cast<float>(timeThisFrameBegan - s_timeLastFrameBegan); //In seconds

	s_timeLastFrameBegan = timeThisFrameBegan;
	g_theGame->Update(deltaSeconds);
}
void Render() {
	g_theGame->Render();

	SwapBuffers(g_displayDeviceContext);
}
void RunFrame() {
	if (DEBUG_FrameMustBeEnabled) {
		RunMessagePump();
		if (g_theInputSystem->GetKeyDown(VK_F1)) {
			Update();
			Render();
		}
		else if (g_theInputSystem->GetKey(VK_F2)) {
			Update();
			Render();
		}
	}
	else {
		RunMessagePump();
		Update();
		Render();
	}
}

//-----------Initialization and shutdown-------------------
void Initialize(HINSTANCE applicationInstanceHandle) {
	g_theApp = new TheApp(ScreenCoords(1600, 900));

	SetProcessDPIAware();
	CreateOpenGLWindow(applicationInstanceHandle);

	g_theRenderer = new TheRenderer();
	g_theInputSystem = new InputSystem();
	g_theGame = new TheGame();
}
void Shutdown() {
	delete g_theGame;
	g_theGame = nullptr;
	delete g_theInputSystem;
	g_theInputSystem = nullptr;
	delete g_theApp;
	g_theApp = nullptr;
	delete g_theRenderer;
	g_theRenderer = nullptr;
}

//Driver
int WINAPI WinMain(HINSTANCE applicationInstanceHandle, HINSTANCE, LPSTR commandLineString, int) {
	UNUSED(commandLineString);
	Initialize(applicationInstanceHandle);

	while (!g_theInputSystem->HasQuit())
		RunFrame();

	Shutdown();
	return 0;
}