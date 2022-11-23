#pragma once

#include <assert.h>

#include "ComponentsEnum.h"
#include "../External/assimp/code/AssetLib/FBX/FBXDocument.h"
#include "JsonInclude.h"
//#include "MonoObjects/MonoComponent.h"

class Component;
class MonoComponent;
class Actor;
class Game;

typedef std::string name;

class Component
{
	friend Actor;
	friend Game;

public:
	Component();

	virtual void Init()
	{
	};

	virtual void OnRegister()
	{
	};

	virtual void Update(float DeltaTime)
	{
	};

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
	uuid GetId() const { return id; }
	std::string GetName() const { return name; }

	virtual json Serialize() const = 0;
	virtual void Deserialize(const json* in) = 0;
private:
	Actor* mOwner;
	uuid id;
	std::string name;
};