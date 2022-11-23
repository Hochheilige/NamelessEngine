#include <cstdint>

#include "Keyboard.h"
#include "Game.h"
#include "MonoSystem.h"

Keyboard::Keyboard()
{
	auto mono = MonoSystem::GetInstance();
	auto klass = mono->FindClass("Scripts", "InputHandler");
	auto instance = mono->CreateClassInstance(klass);
	Handle = mono_gchandle_new(instance, true);
}

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
			MonoMethod* method = mono->GetMethod("Scripts", "InputHandler", "FireKeyboardEvent");
			mono->InvokeMethod(method, mono_gchandle_get_target (Handle), args, nullptr);
		
			return;
		}
	case WM_KEYUP:
		{
			pressedKeys.erase(wParam);
			/*
			void *args [1];
			args [0] = &wParam;
			auto mono = MonoSystem::GetInstance();
			MonoMethod* method = mono->GetMethod("Scripts", "InputHandler", "RemovePressedKey");
			mono->InvokeMethod(method, mono_gchandle_get_target (Handle), args, nullptr);*/
		}
		return;
	default:
		// Not a keyboard message, so exit
		return;
	}
}
