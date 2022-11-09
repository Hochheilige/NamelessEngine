#pragma once

#include <vector>

#include "Game.h"
#include "Object.h"
#include "Transform.h"
#include "Component.h"
#include "SceneComponent.h"
#include "MonoObjects/MonoActor.h"

class LineRenderer;
class MeshRenderer;
class RigidBodyComponent;

class Actor : public Object
{
public:

	friend class ImGuiSubsystem;

	Actor();

	auto GetTransform() const -> const Transform& { return RootComponent ? RootComponent->GetTransform() : Transform::Identity; }
	auto SetTransform(const Transform& InTransform) const -> void { if (RootComponent)  RootComponent->SetRelativeTransform(InTransform); else assert(false); /*attempting to set transform when actor doesn't have a root component*/ }

	template<typename T>
	auto AddComponent() -> T*
	{
		static_assert(std::is_base_of_v<Component, T>, "Only componentns can be added to an actor");

		T* component = nullptr;

		// todo: do we need this?
		if constexpr (std::is_same<T, MeshRenderer>())
		{
			is_mesh_renderer_enabled = true;
		}		
		
		if constexpr (std::is_base_of<LineRenderer, T>())
		{
			is_debug_renderer_enabled = true;
		}

		component = new T();
		Components.push_back(component);
		component->mOwner = this;

		if constexpr (std::is_base_of<SceneComponent, T>())
		{
			if (RootComponent == nullptr)
			{
				RootComponent = component;
			}
			else
			{
				// Attach to RootComponent by default
				component->SetAttachmentParent(RootComponent);
			}
		}
		
		mMonoActor->AddComponent(component);
		//auto a = mMonoActor->GetInheritors();

		return component;
	}

	template<typename T>
	void RemoveComponentsOfClass()
	{
		Components.erase(remove_if(Components.begin(), Components.end(),
			[](Component* comp) {return dynamic_cast<T*>(comp); }), Components.end());
	}

	template<typename T>
	T* GetComponentOfClass()
	{
		T* comp = dynamic_cast<T*>(*std::find_if(Components.begin(), Components.end(),
			[](Component* comp) {return dynamic_cast<T*>(comp); }));
		return comp;
	}

	auto RemoveComponent(Component* InComponent) -> void;

	auto GetComponentsArray()->const std::vector<Component*> { return Components; };

	void RemoveChild(Actor* Child);

	void UseDebugRendererOnly();
	void UseMeshRendererOnly();
	void UseDebugAndMeshRenderer();

	// TODO change naming
	void UsePhysicsSimulation();
	void UnUsePhysicsSimulation();

	virtual void Update(float DeltaTime) override;

	auto GetRoot() const -> SceneComponent* {
		return RootComponent;
	}

private:
	Actor* Parent = nullptr;

	// todo: Think about being able to update Root at Runtime
	SceneComponent* RootComponent = nullptr;
	std::vector<Component*> Components;

	MonoActor* mMonoActor;
	
public:
	bool is_physics_enabled = false;
	bool is_debug_renderer_enabled = false;
	bool is_mesh_renderer_enabled = false;

private:
	
	friend class Game;
	friend class Component;
};