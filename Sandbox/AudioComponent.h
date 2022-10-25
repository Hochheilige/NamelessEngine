#pragma once
#include "MonoSystem.h"
#include <Transform.h>

class AudioComponent //Нужен будет общий интерфейс IComponent
{
public:
	AudioComponent(MonoSystem* mono);
	void Init();
	void OnRegister();
	void GetTransform(MonoSystem* mono);

	//Что-то такое должно быть
	//GameObject GetParent();
	private:
	MonoSystem* _mono;

	MonoClass* _klass;

	MonoObject* _instance;
};

