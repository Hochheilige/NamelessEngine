#pragma once

#include <vector>

#include "Game.h"
#include "Object.h"
#include "Transform.h"
#include "Component.h"

class LineRenderer;
class MeshRenderer;
class RigidBodyComponent;

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
	T* AddComponent(float mass = 0.0f)
	{
		T* component = nullptr;

		auto component_on_actor = std::find_if(Components.begin(), Components.end(),
			[](Component* comp) {return dynamic_cast<T*>(comp); }
		);
		if (component_on_actor != Components.end())
		{
			printf("[Error]: Component of this type {%s} already exists", typeid(T).name());
			return component;
		}

		if (std::is_same<T, MeshRenderer>())
		{
			is_mesh_renderer_enable = true;
		}		
		
		if (std::is_base_of<LineRenderer, T>())
		{
			is_debug_renderer_enable = true;
		}

		component = new T(mTransform);
		Components.push_back(component);
		return component;
	}

	template<typename T>
	void RemoveComponent()
	{
		Components.erase(remove_if(Components.begin(), Components.end(),
			[](Component* comp) {return dynamic_cast<T*>(comp); }), Components.end());
	}

	void RemoveChild(Actor* Child);

	void UseDebugRendererOnly();
	void UseMeshRendererOnly();
	void UseDebugAndMeshRenderer();

	// TODO change naming
	void UsePhysicsSimulation();
	void UnUsePhysicsSimulation();

	virtual void Update(float DeltaTime) override;

private:
	Actor* Parent = nullptr;

	std::shared_ptr<Transform> mTransform;
	std::vector<Actor*> Children;
	std::vector<Component*> Components;

	bool is_physics_enable = false;
	bool is_debug_renderer_enable = false;
	bool is_mesh_renderer_enable = false;

	friend class Game;
};