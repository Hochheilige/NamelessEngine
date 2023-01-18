#include "InputDevice.h"

#include "Game.h"
#include <iostream>
#include <windowsx.h>
#include "DisplayWin32.h"
#include "AssetManager.h"

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
	case WM_DPICHANGED:
		//if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
		{
			//const int dpi = HIWORD(wParam);
			//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
			const RECT* suggested_rect = (RECT*)lparam;
			::SetWindowPos(Game::GetInstance()->GetDisplay()->GetWindowHandle(), NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
		}
		return 0;
	case WM_SETFOCUS:
	{
		// todo: make a application focused callback func in Game and move this there
		AssetManager* am = Game::GetInstance()->GetAssetManager();
		if (am)
		{
			am->Initialize();
		}
		return 0;
	}
	// work in progress
	//case WM_SETCURSOR:
	//{
	//	if (LOWORD(lparam) == HTCLIENT)
	//	{
	//		if (!Game::GetInstance()->GetShowMouseCursor())
	//		{
	//			SetCursor(NULL);
	//			return TRUE;
	//		}
	//	}
	//	return FALSE;
	//}
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
