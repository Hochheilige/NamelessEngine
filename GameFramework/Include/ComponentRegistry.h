#pragma once
#include "FactoryRegistry.h"
#include <string>
#include "ComponentsEnum.h"

class Component;

class ComponentRegistry : public FactoryRegistry<ComponentType, Component>
{
public:
	static ComponentType GetTypeByName(std::string& name);
	static std::string GetNameByType(ComponentType type);
	static Component* CreateByName(std::string& name);
	static Component* CreateByType(ComponentType type);
	static void Init();
	static void Validate();
private:
	ComponentRegistry(): FactoryRegistry() {}

	static void Register(ComponentType type, const std::string& name, FactoryFunction factory);
	static auto GetInstance() -> ComponentRegistry*
	{
		if(Instance == nullptr) {
			Instance = new ComponentRegistry();
		}

		return Instance;
	}

	static ComponentRegistry* Instance;
	static std::unordered_map<ComponentType, std::string> TypeToName;
	static std::unordered_map<std::string, ComponentType> NameToType;
};