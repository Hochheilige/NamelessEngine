#pragma once
#include "Transform.h"
#include "../Actor.h"
#include "../MonoSystem.h"
#include "../RigidBodyComponent.h"


class MonoRigidBodyComponentModule
{
public:
    MonoRigidBodyComponentModule()
    {
        mono_add_internal_call("Scripts.Components.RigidBodyComponent::InternalSetMass", &SetMass);
        mono_add_internal_call("Scripts.Components.RigidBodyComponent::InternalSetGravity", &SetGravity);
        mono_add_internal_call("Scripts.Components.RigidBodyComponent::InternalSetLinearVelocity", &SetLinearVelocity);
        mono_add_internal_call("Scripts.Components.RigidBodyComponent::InternalApplyCentralImpulse", &ApplyCentralImpulse);
        mono_add_internal_call("Scripts.Components.RigidBodyComponent::InternalEnablePhysicsSimulation", &EnablePhysicsSimulation);
        mono_add_internal_call("Scripts.Components.RigidBodyComponent::InternalDisablePhysicsSimulation", &DisablePhysicsSimulation);
        mono_add_internal_call("Scripts.Components.RigidBodyComponent::InternalSetType", &SetType);
        mono_add_internal_call("Scripts.Components.RigidBodyComponent::InternalSetUsage", &SetUsage);
        mono_add_internal_call("Scripts.Components.RigidBodyComponent::InternalSetCollisionShape", &SetCollisionShape);
    }

private:
    static void SetMass(RigidBodyComponent* component, float mass){component->SetMass(mass);}
    static void SetGravity(RigidBodyComponent* component, float gravity){component->SetGravity(gravity);}
    static void SetLinearVelocity(RigidBodyComponent* component, Vector3 velocity)
    {
        auto x = velocity.x;
        component->SetLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
    }
    static void ApplyCentralImpulse(RigidBodyComponent* component, Vector3 impulse) {
        component->applyCentralImpulse(impulse);
    }

    static void EnablePhysicsSimulation(RigidBodyComponent* component) { component->EnablePhysicsSimulation(); }
    static void DisablePhysicsSimulation(RigidBodyComponent* component) { component->DisablePhysicsSimulation(); }

    static void SetType(RigidBodyComponent* component, RigidBodyType type) { component->SetRigidBodyType(type); }
    static void SetUsage(RigidBodyComponent* component, RigidBodyUsage usage) { component->SetRigidBodyUsage(usage); }
    static void SetCollisionShape(RigidBodyComponent* component, CollisionShapeType type) { component->SetCollisionShape(type); }
};