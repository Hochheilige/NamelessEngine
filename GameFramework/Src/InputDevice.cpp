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

	case WM_MOUSEMOVE:
	{
		DisplayWin32* disp = Game::GetInstance()->GetDisplay();
		const int posX = GET_X_LPARAM(lparam);
		const int posY = GET_Y_LPARAM(lparam);
		MouseMoveDeltaX = (PrevPosX - posX) / static_cast<float>(disp->GetClientWidth());
		MouseMoveDeltaY = (PrevPosY - posY) / static_cast<float>(disp->GetClientHeight());
		PrevPosX = posX;
		PrevPosY = posY;
		return 0;
	}
	default:
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
}

void InputDevice::PreInputProcess()
{
	MouseMoveDeltaX = 0.0f;
	MouseMoveDeltaY = 0.0f;
}

void InputDevice::GetMouseDelta(float& OutX, float& OutY)
{
	OutX = MouseMoveDeltaX;
	OutY = MouseMoveDeltaY;
}
