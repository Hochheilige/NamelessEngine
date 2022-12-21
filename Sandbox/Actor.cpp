#include "Actor.h"

#include "MeshRenderer.h"
#include "LineRenderer.h"
#include "RigidBodyComponent.h"
#include "AudioComponent.h"
#include "RenderingSystem.h"

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

	if(mMonoActor)
	{
		mMonoActor->Update(DeltaTime);
	}
	
}

Actor::Actor()
{
}

auto Actor::RemoveComponent(Component* InComponent) -> void
{
	if (InComponent == nullptr)
	{
		return;
	}

	Components.erase(remove(Components.begin(), Components.end(), InComponent), Components.end());
}

void Actor::InitializeMonoActor(const char* name)
{
	mMonoActor = new MonoActor(this, name);
}

void Actor::OnBeginPlay()
{
	if(mMonoActor)
	{
		mMonoActor->OnBeginPlay();
	}
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
		physics->GetDynamicsWorld()->addRigidBody(rigid_body->GetRigidBody());
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
		physics->GetDynamicsWorld()->removeRigidBody(rigid_body->GetRigidBody());
	}
}

void Actor::Overlap()
{

}


void callback(btDynamicsWorld* world, btScalar timeSleep)
{
	auto ghostObjects = PhysicsModuleData::GetInstance()->GetGhostObjects();
	for (auto ghost : ghostObjects)
	{
		if (ghost->getNumOverlappingObjects())
		{
			auto actor = reinterpret_cast<RigidBodyComponent*>(ghost->getUserPointer())->GetOwner();
			for (int i = 0; i < ghost->getNumOverlappingObjects(); ++i)
			{
				// We can get object that this object overlapp with
				// I think that we should find somehow Actors of this objects
				// and do something that we need on this callback
				btCollisionObject* rb = ghost->getOverlappingObject(i);

				actor->Overlap();

			}
		}
	}
}