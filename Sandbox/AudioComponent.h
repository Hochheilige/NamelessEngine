#pragma once
#include "MonoSystem.h"

class AudioComponent //����� ����� ����� ��������� IComponent
{
public:
	AudioComponent(MonoImage* image);
	void Init();
	void OnRegister();

	//���-�� ����� ������ ����
	//GameObject GetParent();


	MonoClass* ptrMainClass;
};

