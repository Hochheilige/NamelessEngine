#pragma once

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
	virtual void Init() = 0;
	virtual void OnRegister() = 0;
	virtual void Update(float DeltaTime) = 0;

	virtual MonoComponent* GetMonoComponent()
	{
		return  nullptr;
	};
	virtual ComponentType GetComponentType()
	{
		return ComponentType::MeshRendererType;
	};
	
	Actor* GetOwner() const { return mOwner; }

private:

	Actor* mOwner;
};