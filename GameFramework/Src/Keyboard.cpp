#include <cstdint>

#include "Keyboard.h"
#include "Game.h"

bool Keyboard::IsDown(uint64_t key)
{
	return pressedKeys.find(key) != pressedKeys.end();
}

void Keyboard::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == KEY_ESC)
		{
			Game::GetInstance()->Exit();
			return;
		}

		pressedKeys.insert(wParam);

		return;
	case WM_KEYUP:
		pressedKeys.erase(wParam);
		return;
	default:
		// Not a keyboard message, so exit
		return;
	}
}
