#pragma once

#include "MovementComponent.h"

class MonoMovementComponentModule
{
public:
    MonoMovementComponentModule()
    {
        mono_add_internal_call("Scripts.Components.MovementComponent::InternalJump", &jump);
        mono_add_internal_call("Scripts.Components.MovementComponent::InternalSetLinearVelocity", &SetLinearVelocity);
        mono_add_internal_call("Scripts.Components.MovementComponent::InternalSetWalkDirection", &setWalkDirection);
        
    }

private:
    static void setStepHeight(MovementComponent* component, float height) { component->setStepHeight(height); }
    static void setjumpSpeed(MovementComponent* component, float speed) { component->setJumpSpeed(speed); }
    static void SetLinearVelocity(MovementComponent* component, Vector3 velocity)
    {
        component->setLinearVelocity(velocity);
    }
    static void jump(MovementComponent* component) { component->jump(); }
    static void setWalkDirection(MovementComponent* component, Vector3 direction) { component->setWalkDirection(direction); }
};