#pragma once

#include <Windows.h>

class DisplayWin32
{
public:
	DisplayWin32(LONG ClientHeight, LONG ClientWidth, WNDPROC WndProc, LPCWSTR ApplicationName);

	HWND GetWindowHandle();

	LONG GetClientHeight() const;
	LONG GetClientWidth() const;

	float GetAspectRatio() const;

private:
	LONG ClientHeight;
	LONG ClientWidth;
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASSEX wc;
	RECT windowRect;
};

