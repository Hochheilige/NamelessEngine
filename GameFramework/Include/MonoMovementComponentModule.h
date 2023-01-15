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
        mono_add_internal_call("Scripts.Components.MovementComponent::InternalSetGravity", &setWalkDirection);
        mono_add_internal_call("Scripts.Components.MovementComponent::InternalGetLinearVelocity", &GetLinearVelocity);
        mono_add_internal_call("Scripts.Components.MovementComponent::InternalCanJump", &CanJump);
        mono_add_internal_call("Scripts.Components.MovementComponent::InternalMoveTo", &MoveTo);
        mono_add_internal_call("Scripts.Components.MovementComponent::InternalStopMoveTo", &StopMoveTo);
        
    }

private:
    static void setStepHeight(MovementComponent* component, float height) { component->setStepHeight(height); }
    static void setjumpSpeed(MovementComponent* component, float speed) { component->setJumpSpeed(speed); }
    static void SetLinearVelocity(MovementComponent* component, Vector3 velocity)
    {
        component->setLinearVelocity(velocity);
    }
    static void jump(MovementComponent* component, Vector3 direction) { component->jump(direction); }
    static bool CanJump(MovementComponent* component) { return component->canJump(); }
    static void setWalkDirection(MovementComponent* component, Vector3 direction) { component->setWalkDirection(direction); }
    static void setGravity(MovementComponent* component, Vector3 gravity) { component->setGravity(gravity); }
    static Vector3 GetLinearVelocity(MovementComponent* component) { return component->getLinearVelocity(); }

	static bool MoveTo(MovementComponent* component, Vector3 moveToPosition) { return component->TryMoveTo(moveToPosition); }
	static void StopMoveTo(MovementComponent* component) { component->StopMoveTo(); }
};