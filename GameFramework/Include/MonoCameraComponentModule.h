#pragma once

#include "CameraComponent.h"

class MonoCameraComponentModule
{
public:
	MonoCameraComponentModule()
	{
		mono_add_internal_call("Scripts.Components.CameraComponent::SetCameraTransformInternal", &SetCameraTransformInternal);
	}

private:
	static void SetCameraTransformInternal(CameraComponent* component, Vector3 position, Quaternion rotationQuaternion) { 
		Transform t;
		t.Position = position;
		t.Rotation = rotationQuaternion;
		component->SetTransform(t); 
	}

};