#include "Actor.h"

#include "MeshRenderer.h"
#include "LineRenderer.h"
#include "RigidBodyComponent.h"

void Actor::Update(float DeltaTime)
{
	if (is_physics_enabled)
	{
		for (auto component : Components)
		{
			if (RigidBodyComponent* comp = dynamic_cast<RigidBodyComponent*>(component))
			{
				comp->Update(DeltaTime);
			}
		}

		for (auto component : Components)
		{
			if (dynamic_cast<RigidBodyComponent*>(component) == nullptr)
			{
				component->Update(DeltaTime);
			}
		}
	}
	else
	{
		for (auto component : Components)
		{
			component->Update(DeltaTime);
		}
	}
}

Actor::Actor()
{
	mMonoActor = new MonoActor();
}

auto Actor::RemoveComponent(Component* InComponent) -> void
{
	if (InComponent == nullptr)
	{
		return;
	}

	Components.erase(remove(Components.begin(), Components.end(), InComponent), Components.end());
}

void Actor::UseDebugRendererOnly()
{
	is_debug_renderer_enabled = true;
	if (is_mesh_renderer_enabled)
	{
		auto mesh_renderer = std::find_if(Components.begin(), Components.end(),
			[](Component* comp) {return dynamic_cast<MeshRenderer*>(comp); }
		);
		Game::GetInstance()->MyRenderingSystem->UnregisterRenderer(
			dynamic_cast<MeshRenderer*>(*mesh_renderer)
		);
		is_mesh_renderer_enabled = false;
	}
}

void Actor::UseMeshRendererOnly()
{
	is_mesh_renderer_enabled = true;
	if (is_debug_renderer_enabled)
	{
		auto line_renderer = std::find_if(Components.begin(), Components.end(),
			[](Component* comp) {return dynamic_cast<LineRenderer*>(comp); }
		);
		Game::GetInstance()->MyRenderingSystem->UnregisterRenderer(
			dynamic_cast<LineRenderer*>(*line_renderer)
		);
		is_debug_renderer_enabled = false;
	}
}

void Actor::UseDebugAndMeshRenderer()
{
}

void Actor::UsePhysicsSimulation()
{
	if (!is_physics_enabled)
	{
		is_physics_enabled = true;
		auto rigid_body = dynamic_cast<RigidBodyComponent*>(*std::find_if(Components.begin(), Components.end(),
			[](Component* comp) {return dynamic_cast<RigidBodyComponent*>(comp); }
		));

		auto physics = PhysicsModuleData::GetInstance();
		physics->GetDynamicsWorls()->addRigidBody(rigid_body->GetRigidBody());
	}
}

void Actor::UnUsePhysicsSimulation()
{
	if (is_physics_enabled)
	{
		is_physics_enabled = false;
		auto rigid_body = dynamic_cast<RigidBodyComponent*>(*std::find_if(Components.begin(), Components.end(),
			[](Component* comp) {return dynamic_cast<RigidBodyComponent*>(comp); }
		));

		auto physics = PhysicsModuleData::GetInstance();
		physics->GetDynamicsWorls()->removeRigidBody(rigid_body->GetRigidBody());
	}
}

