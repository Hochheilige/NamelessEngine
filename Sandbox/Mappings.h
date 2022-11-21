#pragma once
#include "Transform.h"
#include <iostream>

#include "Actor.h"
#include "CreateCommon.h"
#include "RigidBodyCube.h"
#include "RigidBodySphere.h"

class Mappings
{
public:
    //TODO move to nested classes
    static void CS_CreateObj(float trans_x, float trans_y, float trans_z,
        float rot_x, float rot_y, float rot_z,
        float scale_x, float scale_y, float scale_z);
    static void CS_AudioOnCreate();

    static void CubeSetMass(RigidBodyCube* component, float mass){component->SetMass(mass);}
    static void SphereSetGravity(RigidBodySphere* component, float mass){component->SetMass(mass);}
    static Transform ComponentGetTransform(SceneComponent* component){return component->GetTransform();}
    static void ComponentSetTransform(SceneComponent* component, Transform transform){return component->SetTransform(transform);}
    static Component* CreateComponent(Actor* actor, int compType){return actor->AddComponent<RigidBodyComponent>();}
    /*static void AddComponent(Actor* actor, ComponentType type)
    {
        actor->AddComponent<RigidBodyComponent>();
    };*/
    
    /*static Actor* CreateActor()
    {
        Actor* act = CreateActor<Actor>();
        return act;
    }*/
};

