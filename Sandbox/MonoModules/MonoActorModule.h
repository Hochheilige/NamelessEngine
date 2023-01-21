#pragma once
#include "Transform.h"
#include "../MonoSystem.h"
#include "../Actor.h"

#define EXPOSE_ACTOR_FUNC_RETURN_VAL(func_name) \
	 EXPOSE_FUNC_RETURN_VAL(Actor, Scripts.Actor, func_name)

#define EXPOSE_ACTOR_FUNC_ONE_PARAM_NO_RETURN(func_name, param1_type) \
	 EXPOSE_FUNC_ONE_PARAM_NO_RETURN(Actor, Scripts.Actor, func_name, param1_type)

#define EXPOSE_ACTOR_LAMBDA_FUNC(func_name, lambda) \
	 EXPOSE_LAMBDA_FUNC(Scripts.Actor, func_name, lambda)

class MonoActorModule
{
public:

	static auto AddInternalCalls() -> void
	{
		EXPOSE_ACTOR_FUNC_RETURN_VAL(GetTransform);
		EXPOSE_ACTOR_FUNC_ONE_PARAM_NO_RETURN(SetTransform, Transform);
		EXPOSE_ACTOR_LAMBDA_FUNC(Destroy, ([](Actor* actor) {Game::GetInstance()->DestroyActor(actor->AsSharedPtr()); }));
		// todo: move this to some other module (master module?)
		EXPOSE_ACTOR_LAMBDA_FUNC(SetShowMouseCursor, ([](Actor* actor, bool InShowMouseCursor) {Game::GetInstance()->SetShowMouseCursor(InShowMouseCursor); }));
		mono_add_internal_call("Scripts.Engine.Instantiator::InstantiateActorInternal", &InstantiateActor);
	}

	// keep this for legacy purposes
    MonoActorModule()
    {
		AddInternalCalls();
    }

private:
    
    static MonoObject* InstantiateActor(MonoObject* ns, MonoObject* name)
    {
        auto nsStr = mono_string_to_utf8(mono_object_to_string(ns, nullptr));
        auto nameStr = mono_string_to_utf8(mono_object_to_string(name, nullptr));
        auto actor = CreateActor<Actor>();
        actor->InitializeMonoActor(nsStr, nameStr, true);
        
        auto csActor = actor->GetMonoActor();
        return csActor->GetCsInstance();
    }
};
