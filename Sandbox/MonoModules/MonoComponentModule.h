#pragma once
#include "Transform.h"
#include "../Actor.h"
#include "../MonoSystem.h"
#include "../RigidBodyComponent.h"
#include "MovementComponent.h"
#include "ComponentRegistry.h"


class MonoComponentModule
{
public:
    MonoComponentModule()
    {
        mono_add_internal_call("Scripts.Component::InternalCreateComponent", &CreateComponent);
        mono_add_internal_call("Scripts.Component::InternalSetName", &SetName);
        mono_add_internal_call("Scripts.Component::InternalGetName", &GetName);
        mono_add_internal_call("Scripts.Component::InternalGetTransform", &ComponentGetTransform);
        mono_add_internal_call("Scripts.Component::InternalSetTransform", &ComponentSetTransform);
    }

private:
    static Component* CreateComponent(Actor* actor, int compType)
    {
        auto* cmp = ComponentRegistry::CreateByType(static_cast<ComponentType>(compType));
        return actor->AddComponent(cmp);
       
    }
    static void SetName(Component* component, MonoObject* name)
    {
        auto str = mono_string_to_utf8(mono_object_to_string(name, nullptr));
        component->SetName(str);
    }

    static MonoString* GetName(Component* component) {
        return mono_string_new(mono_domain_get(), component->GetName().c_str());
    }

    static Transform ComponentGetTransform(SceneComponent* component){return component->GetTransform();}
    static void ComponentSetTransform(SceneComponent* component, Transform transform)
    {
        component->SetTransform(transform);
    }

};
