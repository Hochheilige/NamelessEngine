#include "OrbitCameraController.h"

#include "InputDevice.h"


OrbitCameraController::OrbitCameraController()
{
	bShouldUpdate = true;
}

void OrbitCameraController::SetCameraToControl(Camera* InCamera)
{
	Cam = InCamera;
}

void OrbitCameraController::Update(float DeltaTime)
{
	if (Cam == nullptr)
	{
		return;
	}

	InputDevice& input = *Game::GetInstance()->GetInputDevice();

	float orbitRadiusDelta = 0.0f;

	if (input.IsKeyDown(69))
	{
		orbitRadiusDelta += DeltaTime * Speed;
	}
	if (input.IsKeyDown(81))
	{
		orbitRadiusDelta -= DeltaTime * Speed;
	}

	float deltaX;
	float deltaY;
	input.GetMouseDelta(deltaX, deltaY);

	Pitch += DeltaTime * deltaY * RotSpeedPitch;
	Pitch = Pitch < -MaxPitch ? -MaxPitch : Pitch > MaxPitch ? MaxPitch : Pitch;
	Yaw += DeltaTime * deltaX * RotSpeedYaw;

	OrbitRadius += orbitRadiusDelta;
	
	// todo: ignore only GCToOrbit's yaw and pitch rotation and scale
	Cam->Transform = Matrix::CreateTranslation(Vector3(0.0f, 0.0f, OrbitRadius)) * 
		Matrix::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(Yaw), DirectX::XMConvertToRadians(Pitch), 0.0f) * 
		Matrix::CreateTranslation(GCToOrbit->GetWorldTransform().Position);
}
