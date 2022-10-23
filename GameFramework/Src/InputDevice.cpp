#include "InputDevice.h"

#include "Game.h"
#include <iostream>
#include <windowsx.h>
#include "DisplayWin32.h"

bool InputDevice::IsKeyDown(uint64_t Key)
{
    return PressedKeys.find(Key) != PressedKeys.end();
}

LRESULT InputDevice::HandleMessage(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_KEYDOWN:
		std::cout << "Key: " << wparam << std::endl;

		if (wparam == 27)
		{
			Game::GetInstance()->Exit();
			return 0;
		}

		PressedKeys.insert(wparam);
		
		return 0;
	case WM_KEYUP:
		PressedKeys.erase(wparam);
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
	default:
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
}

Mouse* InputDevice::GetMouse()
{
	return &mouse;
}

void InputDevice::Prepare()
{
	mouse.Prepare();
}
