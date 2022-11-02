#pragma once

#include "ComponentsEnum.h"
//#include "MonoObjects/MonoComponent.h"

class MonoComponent;
class Actor;
class Game;

class Component
{
	friend Actor;
	friend Game;

public:
	virtual void Init() = 0;
	virtual void OnRegister() = 0;
	virtual void Update(float DeltaTime) = 0;

	virtual MonoComponent* GetMonoComponent();
	virtual ComponentType GetComponentType();
	
	Actor* GetOwner() const { return mOwner; }

private:

	Actor* mOwner;
};