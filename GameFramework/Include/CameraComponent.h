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

	auto UpdateAspectRatio(float newAspectRatio) -> void;

	auto GetMouseDeltaX() -> float;
	auto GetMouseDeltaY() -> float;

protected:
	mutable Camera povData;
};