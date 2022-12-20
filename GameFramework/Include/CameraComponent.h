#pragma once

#include "SceneComponent.h"
#include "MonoCameraComponent.h"

#include "Camera.h"

class CameraComponent : public SceneComponent
{
public:

	CameraComponent();

	ComponentType GetComponentType() override { return CameraComponentType; }

	MonoComponent* GetMonoComponent() override { return mMonoComponent; }


	static auto Create() -> Component* {
		return new CameraComponent();
	}


	MonoCameraComponent* mMonoComponent = new MonoCameraComponent();

	auto GetPOVData() const -> const Camera&;

	auto UpadateAspectRatio(float newAspectRatio) -> void;

protected:
	mutable Camera povData;
};