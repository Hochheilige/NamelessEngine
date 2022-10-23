#pragma once

#include <unordered_set>
#include <Windows.h>
#include "Mouse.h"

class InputDevice
{
public:
	bool IsKeyDown(uint64_t Key);

	LRESULT HandleMessage(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

	Mouse* GetMouse();
	void Prepare();
private:
	Mouse mouse;
	std::unordered_set<uint64_t> PressedKeys;
};

