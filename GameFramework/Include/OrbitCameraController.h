#pragma once

#include "GameComponent.h"

class OrbitCameraController : public GameComponent
{
public:
	OrbitCameraController();

	void SetCameraToControl(class Camera* InCamera);
	virtual void Update(float DeltaTime) override;

	float Speed = 3.0f;

	float OrbitRadius = 10.0f;

	class SceneComponent* SCToOrbit = nullptr;

	auto SetSpeed(float s) -> void { characterSpeed = s; }

private:

	class Camera* Cam = nullptr;

	float Pitch = 0.0f;
	float Yaw = 0.0f;

	float characterSpeed = 10.0f;

	const float MaxPitch = 85.0f;

	float RotSpeedYaw = 50000.0f;
	float RotSpeedPitch = 30000.0f;
};