#pragma once
#include "Transform.h"

#include "Actor.h"
#include "CreateCommon.h"
#include "RigidBodyComponent.h"

class Mappings
{
public:

    static void CubeSetMass(RigidBodyComponent* component, float mass){component->SetMass(mass);}
    static void SphereSetGravity(RigidBodyComponent* component, float mass){component->SetMass(mass);}
    
    static Transform ComponentGetTransform(SceneComponent* component){return component->GetTransform();}
    static void ComponentSetTransform(SceneComponent* component, Transform transform){return component->SetTransform(transform);}
    
    static Component* CreateComponent(Actor* actor, int compType);
    static Transform ActorGetTransform(Actor* actor){return actor->GetTransform();}
    static void ActorSetTransform(Actor* actor, Transform transform){actor->SetTransform(transform);}
};

