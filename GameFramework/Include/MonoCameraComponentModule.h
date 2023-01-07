#pragma once

#include "CameraComponent.h"

class MonoCameraComponentModule
{
public:
	MonoCameraComponentModule()
	{
		mono_add_internal_call("Scripts.Components.CameraComponent::SetCameraTransformInternal", &SetCameraTransformInternal);
		mono_add_internal_call("Scripts.Components.CameraComponent::GetMouseDeltaXInternal", &GetMouseDeltaXInternal);
		mono_add_internal_call("Scripts.Components.CameraComponent::GetMouseDeltaYInternal", &GetMouseDeltaYInternal);
	}

private:
	static void SetCameraTransformInternal(CameraComponent* component, Vector3 position, Quaternion rotationQuaternion) { 
		Transform t;
		t.Position = position;
		t.Rotation = rotationQuaternion;
		component->SetTransform(t); 
	}
	static float GetMouseDeltaXInternal(CameraComponent* component) { return component->GetMouseDeltaX(); }
	static float GetMouseDeltaYInternal(CameraComponent* component) { return component->GetMouseDeltaY(); }

};