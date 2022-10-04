#pragma once

#include "GameComponent.h"

class OrbitCameraController : public GameComponent
{
public:
	OrbitCameraController();

	void SetCameraToControl(class Camera* InCamera);
	virtual void Update(float DeltaTime) override;

	float Speed = 3.0f;

	float OrbitRadius = 30.0f;

	GameComponent* GCToOrbit = nullptr;

private:

	class Camera* Cam = nullptr;

	float Pitch = 0.0f;
	float Yaw = 0.0f;

	

	const float MaxPitch = 85.0f;

	float RotSpeedYaw = 100000.0f;
	float RotSpeedPitch = 30000.0f;
};