#pragma once
#include "MonoSystem.h"

class AudioComponent //Нужен будет общий интерфейс IComponent
{
public:
	AudioComponent(MonoImage* image);
	void Init();
	void OnRegister();

	//Что-то такое должно быть
	//GameObject GetParent();


	MonoClass* ptrMainClass;
};

