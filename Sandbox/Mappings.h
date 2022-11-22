#pragma once
#include "Transform.h"

#include "Actor.h"
#include "CreateCommon.h"
#include "RigidBodyCube.h"
#include "RigidBodySphere.h"

class Mappings
{
public:

    static void CubeSetMass(RigidBodyCube* component, float mass){component->SetMass(mass);}
    static void SphereSetGravity(RigidBodySphere* component, float mass){component->SetMass(mass);}
    
    static Transform ComponentGetTransform(SceneComponent* component){return component->GetTransform();}
    static void ComponentSetTransform(SceneComponent* component, Transform transform){return component->SetTransform(transform);}
    
    static Component* CreateComponent(Actor* actor, int compType);
    static Transform ActorGetTransform(Actor* actor){return actor->GetTransform();}
    static void ActorSetTransform(Actor* actor, Transform transform){actor->SetTransform(transform);}
};

