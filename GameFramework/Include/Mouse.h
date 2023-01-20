#pragma once
#include <Windows.h>

class InputDevice;

enum Button
{
	LEFT,
	MIDDLE,
	RIGHT
};

class Mouse
{
	friend InputDevice;
public:
	bool IsDown(Button button) const;
	void GetDeltas(float& outX, float& outY);
	auto GetScrollDelta() const -> int { return scrollDelta; }
private:
	void Prepare();
	void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void OnScroll(int scrollDelta);
	void SetPressStateByButton(Button button, bool press);

	bool leftButtonPressed = false;
	bool middleButtonPressed = false;
	bool rightButtonPressed = false;

	int scrollDelta = 0;

	int prevX = 0;
	int prevY = 0;
	float deltaX = 0.0f;
	float deltaY = 0.0f;
};

