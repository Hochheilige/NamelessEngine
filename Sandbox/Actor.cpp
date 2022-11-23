#include "Actor.h"

#include "MeshRenderer.h"
#include "LineRenderer.h"
#include "RigidBodyComponent.h"
#include "RenderingSystem.h"
#include "uuid.h"
#include "UUIDGenerator.h"

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
: id(Game::GetInstance()->GetUuidGenerator()->generate())
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
	mMonoActor = new MonoActor(name);
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

json Actor::Serialize() const
{
	json out = json::object();

	out["id"] = id;

	json componentArr = json::array();
	for (auto component : Components) {
		json wrapper = json::object();
		wrapper["name"] = component->GetName();
		wrapper["id"] = component->GetId();
		wrapper["data"] = component->Serialize();

		if(SceneComponent* sc = dynamic_cast<SceneComponent*>(component)) {
			if(sc->GetAttachmentParent() != nullptr) {
				wrapper["parent"] = sc->GetAttachmentParent()->GetId();
			}
		}

		componentArr.push_back(wrapper);
	}

	out["components"] = componentArr;

	return out;
}

void Actor::Deserialize(const json* in)
{
	assert(in->is_object());

	auto componentArr = in->at("components");
	assert(componentArr.is_array());

	auto componentRegistry = Game::GetInstance()->GetComponentRegistry();

 	std::vector<std::pair<SceneComponent*, uuid>> shouldBeBound;

	for (auto wrapper : componentArr) {
		auto id = wrapper.at("id").get<uuid>();

		bool exists = false;
		for (auto component : Components) {
			if(component->GetId() == id) {
				exists = true;

				auto data = wrapper.at("data");
				component->Deserialize(&data);
			} 
		}

		if(!exists) {
			auto name = wrapper.at("id").get<std::string>();
			Component* component = componentRegistry->CreateInstance(name);

			assert(component != nullptr && "Component was not registered");

			auto data = wrapper.at("data");
			component->Deserialize(&data);

			if (SceneComponent* sc = dynamic_cast<SceneComponent*>(component)) {
				auto parentProp = wrapper.at("parent");
				if(!parentProp.is_null()) {
					auto parentId = parentProp.get<uuid>();
					shouldBeBound.push_back( std::make_pair(sc, parentId));
				}
			}

			Components.push_back(component);
		}
	}

	//Done with extra loop because some of parent tree may not be initialized too
	for (auto child : shouldBeBound) {
		for (auto component : Components) {
			if(component->GetId() == child.second) {
				if(SceneComponent* parent = dynamic_cast<SceneComponent*>(component)) {
					child.first->SetAttachmentParent(parent);
				} else {
					assert(false && "Provided parent id belongs to an object which is not a SceneComponent");
				}

				goto nextChild;
			}
		}

		assert(false && "No parent found with provided id");

		nextChild:;
	}
}

uuid Actor::GetId() const
{
	return id;
}
