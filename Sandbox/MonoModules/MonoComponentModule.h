#pragma once
#include "Transform.h"
#include "../Actor.h"
#include "../MonoSystem.h"
#include "../RigidBodyComponent.h"
#include "../RigidBodyCube.h"

class MonoComponentModule
{
public:
    MonoComponentModule()
    {
        mono_add_internal_call("Scripts.PhysicsComponent::PhysicsSetMass", CubeSetMass);
        mono_add_internal_call("Scripts.Component::InternalCreateComponent", &CreateComponent);
    }

private:
    static void CubeSetMass(RigidBodyComponent* component, float mass){component->SetMass(mass);}
    static Component* CreateComponent(Actor* actor, int compType)
    {
        //return actor->AddComponent<RigidBodyComponent>();
        auto box_rb = actor->AddComponent<RigidBodyCube>();
        Transform tr;
        tr.Position = Vector3(16, 3, 0);
        tr.Rotation.SetEulerAngles(0, 0, 0);
        tr.Scale = Vector3(4.0, 4.0, 20.0);
        actor->SetTransform(tr);
        box_rb->SetRigidBodyType(RigidBodyType::STATIC);
        box_rb->SetMass(0);
        box_rb->Init();

        return box_rb;
    }
};
