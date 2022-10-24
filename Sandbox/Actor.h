#pragma once

#include <vector>

#include "Object.h"
#include "Transform.h"
#include "Component.h"

class Actor : public Object
{
public:

	Actor(Transform transform);

	Transform GetTransform() const;
	Transform GetWorldTransform() const;

	Actor* GetParent() const;
	void   SetParent(Actor* InParent);

	Actor* GetChild(Actor* Child) const;
	void   AddChild(Actor* Child);

	template<typename T>
	void AddComponent()
	{
		Components.push_back(new T(mTransform));
	}

	template<typename T>
	void RemoveComponent()
	{

		Components.erase(remove_if(Components.begin(), Components.end(),
			[](Component* comp) {return dynamic_cast<T>(comp); }), Components.end())
	}

	void RemoveChild(Actor* Child);

	virtual void Update(float DeltaTime) override;

private:
	Actor* Parent = nullptr;

	Transform mTransform;
	std::vector<Actor*> Children;
	std::vector<Component*> Components;
};