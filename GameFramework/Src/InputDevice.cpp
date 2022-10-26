#include "InputDevice.h"

#include "Game.h"
#include <iostream>
#include <windowsx.h>
#include "DisplayWin32.h"
#include <imgui.h>


bool InputDevice::IsKeyDown(uint64_t Key)
{
    return PressedKeys.find(Key) != PressedKeys.end();
}

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

LRESULT InputDevice::HandleMessage(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, umessage, wparam, lparam)) {
		return true;
	}
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
	case WM_SIZE:
	{
		Game::GetInstance()->HandleWindowResize();
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
