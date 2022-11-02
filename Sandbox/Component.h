#pragma once

#include <assert.h>

#include "ComponentsEnum.h"
#include "../External/assimp/code/AssetLib/FBX/FBXDocument.h"
//#include "MonoObjects/MonoComponent.h"

class MonoComponent;
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

	virtual MonoComponent* GetMonoComponent()
	{
		assert(false, "Not Defined");
		return nullptr;
		//throw std::exception("Not Defined");
	}
	
	virtual ComponentType GetComponentType()
	{
		assert(false, "Not Defined");
		return ComponentType::Unddefined;
		//throw std::exception("Not Defined");
	}
	
	Actor* GetOwner() const { return mOwner; }

private:

	Actor* mOwner;
};