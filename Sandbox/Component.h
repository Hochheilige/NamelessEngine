#pragma once

#include "Transform.h"

class Actor;
class Game;

class Component
{
	friend Actor;
	friend Game;

public:
	virtual void Init() {};
	virtual void OnRegister() {};
	virtual void Update(float DeltaTime) {};


	Actor* GetOwner() const { return mOwner; }

private:

	Actor* mOwner;
};