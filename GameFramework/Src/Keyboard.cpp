#include <cstdint>

#include "Keyboard.h"
#include "Game.h"
#include "MonoSystem.h"

bool Keyboard::IsDown(uint64_t key)
{
	return pressedKeys.find(key) != pressedKeys.end();
}

void Keyboard::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		{
			if (wParam == KEY_ESC)
			{
				Game::GetInstance()->Exit();
				return;
			}

			pressedKeys.insert(wParam);
			
			void *args [1];
			args [0] = &wParam;
			auto mono = MonoSystem::GetInstance();
			MonoMethod* method = mono->GetMethod("Scripts.Internal", "InternalApi", "KeyPressed");
			mono->InvokeStaticMethod(method, args, nullptr);
		
			return;
		}
	case WM_KEYUP:
		{
			pressedKeys.erase(wParam);

			void* args[1];
			args[0] = &wParam;
			auto mono = MonoSystem::GetInstance();
			MonoMethod* method = mono->GetMethod("Scripts.Internal", "InternalApi", "KeyReleased");
			mono->InvokeStaticMethod(method, args, nullptr);
		}
		return;
	default:
		// Not a keyboard message, so exit
		return;
	}
}
