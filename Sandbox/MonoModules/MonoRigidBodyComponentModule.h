﻿#pragma once
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
};