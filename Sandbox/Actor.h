#pragma once

#include <vector>

#include "Game.h"
#include "Object.h"
#include "Transform.h"
#include "Component.h"
#include "SceneComponent.h"
#include "MonoObjects/MonoActor.h"
#include "JsonInclude.h"
#include "LineRenderer.h"
#include "MeshRenderer.h"
#include "uuid.h"
#include "AudioComponent.h"


class LineRenderer;
class MeshRenderer;
class RigidBodyComponent;

class Actor final : public Object
{
public:

	friend class ImGuiSubsystem;

	Actor();
	~Actor();

	auto GetTransform() const -> const Transform& { return RootComponent ? RootComponent->GetTransform() : Transform::Identity; }
	auto SetTransform(const Transform& InTransform) const -> void { if (RootComponent)  RootComponent->SetRelativeTransform(InTransform); else assert(false); /*attempting to set transform when actor doesn't have a root component*/ }

	template<typename T>
	auto AddComponent() -> T*
	{
		static_assert(std::is_base_of_v<Component, T>, "Only components can be added to an actor");

		T* component = nullptr;

		component = new T();
		Components.push_back(component);

		OnComponentAdded(component);
		AddOrphanComponent(component);

		return component;
	}

	auto AddComponent(Component* component) -> Component* {
		Components.push_back(component);

		OnComponentAdded(component);
		AddOrphanComponent(component);

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
		auto comp = std::find_if(Components.begin(), Components.end(),
			[](Component* comp) {return dynamic_cast<T*>(comp); });

		if (comp != Components.end())
			return dynamic_cast<T*>(*comp);

		return nullptr;
	}

	auto RemoveComponent(Component* InComponent) -> void;

	auto GetComponentsArray()->const std::vector<Component*> { return Components; };

	void RemoveChild(Actor* Child);

	void SetUuid(uuid in);

	void InitializeMonoActor(const char* className = "Actor");
	void InitializeMonoActor(const char* nameSpace, const char* className);

	MonoActor* GetMonoActor() { return mMonoActor;}

	void OnBeginPlay();

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

	json Serialize() const;
	void Deserialize(const json* in);
	uuid GetId() const;
private:
	void OnComponentAdded(Component* component);
	void AddOrphanComponent(Component* component);

	Actor* Parent = nullptr;

	// todo: Think about being able to update Root at Runtime
	SceneComponent* RootComponent = nullptr;
	std::vector<Component*> Components;

	MonoActor* mMonoActor = nullptr;
	
public:
	bool is_physics_enabled = false;
	bool is_debug_renderer_enabled = false;
	bool is_mesh_renderer_enabled = false;

private:
	uuids::uuid id;
	friend class Game;
	friend class Component;
};