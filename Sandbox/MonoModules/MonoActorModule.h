#pragma once
#include "Transform.h"
#include "../MonoSystem.h"
#include "../Actor.h"

class MonoActorModule
{
public:
    MonoActorModule()
    {
        mono_add_internal_call("Scripts.Actor::InternalGetTransform", &ActorGetTransform);
        mono_add_internal_call("Scripts.Actor::InternalSetTransform", &ActorSetTransform);
        mono_add_internal_call("Scripts.Engine.Instantiator::InstantiateActorInternal", &InstantiateActor);
    }

private:
    static Transform ActorGetTransform(Actor* actor){return actor->GetTransform();}
    static void ActorSetTransform(Actor* actor, Transform transform){actor->SetTransform(transform);}
    static MonoObject* InstantiateActor(MonoObject* name)
    {
        auto str = mono_string_to_utf8(mono_object_to_string(name, nullptr));
        auto actor = CreateActor<Actor>();
        actor->InitializeMonoActor(str);
        //actor->GetMonoActor()->Init();
        
        auto csActor = actor->GetMonoActor();
        return csActor->GetCsInstance();
    }
};
