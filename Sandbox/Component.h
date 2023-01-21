#pragma once

#include <assert.h>

#include "ComponentsEnum.h"
#include "../External/assimp/code/AssetLib/FBX/FBXDocument.h"
#include "JsonInclude.h"
//#include "MonoObjects/MonoComponent.h"
#include <memory>

class Component;
class MonoComponent;
class Actor;
class Game;

//typedef std::string name;

class Component
{
	friend Actor;
	friend Game;

public:
	Component();

	void SetName(const std::string &name) {
		this->name = name;
	}

	const std::string& GetName() const {
		return this->name;
	}

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

	std::weak_ptr<Actor> GetOwner() const { return mOwner; }
	uuid GetId() const { return id; }
	void SetId(uuid idIn)
	{
		id = idIn;
	}

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
	virtual void OnDeserializationCompleted()
	{
	}
private:
	static std::unordered_map<std::string, ComponentType> TYPE_BY_NAME;
	static std::unordered_map<ComponentType, std::string> NAME_BY_TYPE;

	std::weak_ptr<Actor> mOwner;
	uuid id;

	std::string name;
};