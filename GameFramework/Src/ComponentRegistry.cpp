#include "ComponentRegistry.h"

#include <utility>

#include "ComponentsEnum.h"
#include "RigidBodyComponent.h"
#include "SceneComponent.h"
#include "StaticMeshRenderer.h"
#include "LightBase.h"
#include "MovementComponent.h"
#include "AudioComponent.h"
#include "CameraComponent.h"


#include "MeshRenderer.h"

ComponentRegistry* ComponentRegistry::Instance = nullptr;
std::unordered_map<ComponentType, std::string> ComponentRegistry::TypeToName;
std::unordered_map<std::string, ComponentType> ComponentRegistry::NameToType;

ComponentType ComponentRegistry::GetTypeByName(std::string& name)
{
	const auto pair = NameToType.find(name);
	if (pair == NameToType.end()) return Undefined;

	return pair->second;
}

std::string ComponentRegistry::GetNameByType(ComponentType type)
{
	const auto pair = TypeToName.find(type);
	if (pair == TypeToName.end()) return "";

	return pair->second;
}

Component* ComponentRegistry::CreateByName(std::string& name)
{
	return GetInstance()->CreateInstance(GetTypeByName(name));
}

Component* ComponentRegistry::CreateByType(ComponentType type)
{
	return GetInstance()->CreateInstance(type);
}

void ComponentRegistry::Register(ComponentType type, const std::string& name, FactoryFunction factory)
{
	TypeToName.insert(std::make_pair(type, name));
	NameToType.insert(std::make_pair(name, type));
	GetInstance()->RegisterFactory(type, factory);
}

void ComponentRegistry::Init()
{
	Register(SceneComponentType, "SceneComponent", &SceneComponent::Create);
	Register(RigidBodyComponentType, "RigidBodyComponent", &RigidBodyComponent::Create);
	Register(StaticMeshRendererType, "StaticMeshRenderer", &StaticMeshRenderer::Create);
	Register(LightPointType, "PointLight", &PointLight::Create);
	//TODO: remove mesh renderer once it's useless
	Register(MeshRendererType, "MeshRenderer", &MeshRenderer::Create);
	Register(MovementComponentType, "MovementComponent", &MovementComponent::Create);
	Register(AudioComponentType, "AudioComponent", &AudioComponent::Create);
	Register(CameraComponentType, "CameraComponent", &CameraComponent::Create);
	Register(CameraComponentType, "AmbientLight", &AmbientLight::Create);
	Register(CameraComponentType, "DirectionalLight", &DirectionalLight::Create);
}

void ComponentRegistry::Validate()
{
	/*for (int type = 1; type != Last; type++)
	{
		auto cType = static_cast<ComponentType>(type);
		auto s = GetNameByType(cType);
		assert(!s.empty() && "Component Data is not registered for provided ComponentType");
	}*/
}
