#pragma once

#include <unordered_set>
#include <Windows.h>

#include "Keyboard.h"
#include "Mouse.h"

class InputDevice
{
public:
	LRESULT HandleMessage(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

	Mouse* GetMouse();
	Keyboard* GetKeyboard();
	void Prepare();
private:
	Mouse mouse;
	Keyboard keyboard;
};

