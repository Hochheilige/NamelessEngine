#pragma once

#include "Transform.h"

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


	Actor* GetOwner() const { return mOwner; }

private:

	Actor* mOwner;
};