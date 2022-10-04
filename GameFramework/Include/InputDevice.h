#pragma once

#include <unordered_set>
#include <Windows.h>

class InputDevice
{
public:
	bool IsKeyDown(uint64_t Key);

	LRESULT HandleMessage(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

	void PreInputProcess();

	void GetMouseDelta(float& OutX, float& OutY);

private:
	std::unordered_set<uint64_t> PressedKeys;

	float MouseMoveDeltaX = 0.0f;
	float MouseMoveDeltaY = 0.0f;

	int PrevPosX = 0;
	int PrevPosY = 0;
};

