#include "Actor.h"

#include "MeshRenderer.h"
#include "LineRenderer.h"
#include "RigidBodyComponent.h"
#include "ComponentRegistry.h"
#include "AudioComponent.h"
#include "MovementComponent.h"
#include "RenderingSystem.h"
#include "UUIDGenerator.h"

void Actor::Update(float DeltaTime)
{
	if (true)
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

Actor::~Actor()
{
	delete(mMonoActor);

	auto game = Game::GetInstance();
	game->Actors.erase(std::remove(game->Actors.begin(), game->Actors.end(), this), game->Actors.end());
	
	for (auto comp : Components) {
		delete(comp);
	}
}

void Actor::OnComponentAdded(Component* component)
{
	// todo: do we need this?
	if (dynamic_cast<MeshRenderer*>(component))
	{
		is_mesh_renderer_enabled = true;
	}

	if (dynamic_cast<LineRenderer*>(component))
	{
		is_debug_renderer_enabled = true;
	}

	if (mMonoActor != nullptr)
	{
		mMonoActor->AddComponent(component);
	}

	component->mOwner = this;
}

void Actor::AddOrphanComponent(Component* component)
{
	if (auto scc = dynamic_cast<SceneComponent*>(component))
	{
		if (RootComponent == nullptr)
		{
			RootComponent = scc;
		}
		else
		{
			// Attach to RootComponent by default
			scc->SetAttachmentParent(RootComponent);
		}
	}
}

auto Actor::RemoveComponent(Component* InComponent) -> void
{
	if (InComponent == nullptr)
	{
		return;
	}

	Components.erase(remove(Components.begin(), Components.end(), InComponent), Components.end());
}

void Actor::SetUuid(uuid idIn)
{
	id = idIn;
}

void Actor::InitializeMonoActor(const char* className)
{
	mMonoActor = new MonoActor(this, "Scripts", className);
}

void Actor::InitializeMonoActor(const char* nameSpace, const char* className, bool initComponents)
{
	mMonoActor = new MonoActor(this, nameSpace, className);

	if(initComponents) {
		mMonoActor->RegisterComponents();
		mMonoActor->Init();
	}
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


json Actor::Serialize() const
{
	json out = json::object();

	out["id"] = id;

	if (mMonoActor != nullptr) {
		json monoObj = json::object();
		monoObj["namespace"] = mMonoActor->GetNamespace();
		monoObj["class"] = mMonoActor->GetClassname();
		out["mono"] = monoObj;
	}

	json componentArr = json::array();
	for (auto component : Components) {
		json wrapper = json::object();
		const auto type = component->GetComponentType();
		assert(type != Undefined && "Abstract components can't be serialized.");

		wrapper["name"] = ComponentRegistry::GetNameByType(type);
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
	out["name"] = Name;

	return out;
}

void Actor::Deserialize(const json* in, bool destructive)
{
	assert(in->is_object());

	if(in->contains("mono")) {
		auto monoObj = in->at("mono");
		auto namespaceStr = monoObj.at("namespace").get<std::string>();
		auto classnameStr = monoObj.at("class").get<std::string>();
		InitializeMonoActor(namespaceStr.c_str(), classnameStr.c_str(), false);
	}

	auto componentArr = in->at("components");
	assert(componentArr.is_array());

 	std::vector<std::pair<SceneComponent*, uuid>> shouldBeBound;

	for (auto wrapper : componentArr) {
		auto id = wrapper.at("id").get<uuid>();

		bool exists = false;
		for (auto component : Components) {
			if(component->GetId() == id) {
				exists = true;

				auto data = wrapper.at("data");
				component->Deserialize(&data);
				component->OnDeserializationCompleted();
			}
		}

		if(!exists) {
			auto name = wrapper.at("name").get<std::string>();
			Component* component = ComponentRegistry::CreateByName(name);

			assert(component != nullptr && "Component was not registered");

			auto data = wrapper.at("data");
			component->SetId(id);
			component->Deserialize(&data);

			if (SceneComponent* sc = dynamic_cast<SceneComponent*>(component)) {
				if(wrapper.contains("parent")) {
					auto parentId = wrapper.at("parent").get<uuid>();
					shouldBeBound.push_back(std::make_pair(sc, parentId));
				} else {
					OnComponentAdded(component);
					AddOrphanComponent(component);
					component->OnDeserializationCompleted();
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
					OnComponentAdded(child.first);
					child.first->OnDeserializationCompleted();
				} else {
					assert(false && "Provided parent id belongs to an object which is not a SceneComponent");
				}

				goto nextChild;
			}
		}

		assert(false && "No parent found with provided id");

		nextChild:;
	}

	if (in->contains("mono")) {
		mMonoActor->Init();
	}

	Name = in->at("name");
}

uuid Actor::GetId() const
{
	return id;
}

void Actor::Overlap(Actor* otherActor)
{
	mMonoActor->Overlap(otherActor);
}


void callback(btDynamicsWorld* world, btScalar timeSleep)
{
	auto ghostObjects = PhysicsModuleData::GetInstance()->GetGhostObjects();

	btCollisionObjectArray& arr = world->getCollisionObjectArray();
	for (int i = 0; i < arr.size(); ++i)
	{
		if (arr[i]->getInternalType() != btCollisionObject::CO_GHOST_OBJECT)
		{
			continue;
		}
		btGhostObject* ghost = static_cast<btGhostObject*>(arr[i]);
		if (ghost->getNumOverlappingObjects() && ghost->getUserPointer())
		{
			auto actor = reinterpret_cast<SceneComponent*>(ghost->getUserPointer())->GetOwner();

			for (int i = 0; i < ghost->getNumOverlappingObjects(); ++i)
			{
				// We can get object that this object overlapp with
				// I think that we should find somehow Actors of this objects
				// and do something that we need on this callback
				btCollisionObject* rb = ghost->getOverlappingObject(i);

				if (rb->getUserPointer())
				{
					auto otherActor = reinterpret_cast<SceneComponent*>(rb->getUserPointer())->GetOwner();

					actor->Overlap(otherActor);
				}
			}
		}
	}
}