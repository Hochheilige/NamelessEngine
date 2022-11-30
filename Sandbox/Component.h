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
		assert(false && "Not Implemented");
		return nullptr;
		//throw std::exception("Not Defined");
	}

	virtual ComponentType GetComponentType()
	{
		assert(false && "Not Implemented");
		return Undefined;
		//throw std::exception("Not Defined");
	}

	Actor* GetOwner() const { return mOwner; }
	uuid GetId() const { return id; }

	virtual ~Component() = default;

	virtual json Serialize() const
	{
		assert(false && "Not Implemented");
		throw std::exception();
	}
	virtual void Deserialize(const json* in)
	{
		assert(false && "Not Implemented");
	}
	virtual void OnDeserializationCompleted() {}
private:
	static std::unordered_map<std::string, ComponentType> TYPE_BY_NAME;
	static std::unordered_map<ComponentType, std::string> NAME_BY_TYPE;

	Actor* mOwner;
	uuid id;
};