#include "InputDevice.h"

#include "Game.h"
#include <iostream>
#include <windowsx.h>
#include "DisplayWin32.h"

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

LRESULT InputDevice::HandleMessage(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, umessage, wparam, lparam)) {
		return true;
	}
	
	// TODO: check whether ImGui wants to capture cursor or keyboard
	switch (umessage)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
		keyboard.ProcessMessage(umessage, wparam, lparam);
		return 0;
	case WM_ACTIVATE:
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		mouse.ProcessMessage(umessage, wparam, lparam);
		return 0;
	case WM_SIZE:
		{
			UINT width = LOWORD(lparam);
			UINT height = HIWORD(lparam);
			Game::GetInstance()->HandleWindowResize(static_cast<int>(width), static_cast<int>(height));
			return 0;
		}
	default:
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
}

Mouse* InputDevice::GetMouse()
{
	return &mouse;
}

Keyboard* InputDevice::GetKeyboard()
{
	return &keyboard;
}

void InputDevice::Prepare()
{
	mouse.Prepare();
}
