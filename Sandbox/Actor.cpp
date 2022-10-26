#include "Actor.h"

#include "MeshRenderer.h"
#include "LineRenderer.h"
#include "RigidBodyComponent.h"

Actor::Actor(Transform transform) : mTransform(std::make_shared<Transform>(transform))
{}

Transform Actor::GetTransform() const
{
	return *mTransform;
}

Transform Actor::GetWorldTransform() const
{
	if (Parent == nullptr)
	{
		return *mTransform;
	}

	return mTransform->TransformToWorld(Parent->GetWorldTransform());
}

Actor* Actor::GetParent() const
{
	return Parent;
}

void Actor::SetParent(Actor* InParent)
{
	if (InParent == this)
	{
		return;
	}

	if (Parent != nullptr)
	{
		Parent->Children.erase(
			remove(Parent->Children.begin(), Parent->Children.end(), this), Parent->Children.end());
	}

	Parent = InParent;

	if (Parent != nullptr)
	{
		Parent->Children.push_back(this);
	}
}

Actor* Actor::GetChild(Actor* Child) const
{
	auto foundChild = find(Children.begin(), Children.end(), Child);
	if (foundChild == Children.end())
	{
		return nullptr;
	}

	return *foundChild;
}

void Actor::AddChild(Actor* Child)
{
	Children.push_back(Child);
}


void Actor::Update(float DeltaTime)
{
	if (is_physics_enable)
	{
		auto rigid_body_comp = std::find_if(Components.begin(), Components.end(),
			[](Component* comp) {return dynamic_cast<RigidBodyComponent*>(comp); }
		);
		(*rigid_body_comp)->Update(DeltaTime);

		for (auto component : Components)
		{
			if (*rigid_body_comp != component)
				component->Update(DeltaTime);
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

void Actor::RemoveChild(Actor* Child)
{
	Children.erase(remove(Children.begin(), Children.end(), Child), Children.end());
}

void Actor::UseDebugRendererOnly()
{
	is_debug_renderer_enable = true;
	if (is_mesh_renderer_enable)
	{
		auto mesh_renderer = std::find_if(Components.begin(), Components.end(),
			[](Component* comp) {return dynamic_cast<MeshRenderer*>(comp); }
		);
		Game::GetInstance()->MyRenderingSystem->UnregisterRenderer(
			dynamic_cast<MeshRenderer*>(*mesh_renderer)
		);
		is_mesh_renderer_enable = false;
	}
}

void Actor::UseMeshRendererOnly()
{
	is_mesh_renderer_enable = true;
	if (is_debug_renderer_enable)
	{
		auto line_renderer = std::find_if(Components.begin(), Components.end(),
			[](Component* comp) {return dynamic_cast<LineRenderer*>(comp); }
		);
		Game::GetInstance()->MyRenderingSystem->UnregisterRenderer(
			dynamic_cast<LineRenderer*>(*line_renderer)
		);
		is_debug_renderer_enable = false;
	}
}

void Actor::UseDebugAndMeshRenderer()
{
}

void Actor::UsePhysicsSimulation()
{
	if (!is_physics_enable)
	{
		is_physics_enable = true;
		auto rigid_body = dynamic_cast<RigidBodyComponent*>(*std::find_if(Components.begin(), Components.end(),
			[](Component* comp) {return dynamic_cast<RigidBodyComponent*>(comp); }
		));

		auto physics = PhysicsModuleData::GetInstance();
		physics->GetDynamicsWorls()->addRigidBody(rigid_body->GetRigidBody());
	}
}

void Actor::UnUsePhysicsSimulation()
{
	if (is_physics_enable)
	{
		is_physics_enable = false;
		auto rigid_body = dynamic_cast<RigidBodyComponent*>(*std::find_if(Components.begin(), Components.end(),
			[](Component* comp) {return dynamic_cast<RigidBodyComponent*>(comp); }
		));

		auto physics = PhysicsModuleData::GetInstance();
		physics->GetDynamicsWorls()->removeRigidBody(rigid_body->GetRigidBody());
	}
}

