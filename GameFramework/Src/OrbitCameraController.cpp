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

	auto keyboard = input.GetKeyboard();

	if (keyboard->IsDown(KEY_C))
	{
		orbitRadiusDelta += DeltaTime * Speed;
	}
	if (keyboard->IsDown(81))
	{
		orbitRadiusDelta -= DeltaTime * Speed;
	}

	float deltaX;
	float deltaY;
	input.GetMouse()->GetDeltas(deltaX, deltaY);

	Pitch += DeltaTime * deltaY * RotSpeedPitch;
	Pitch = Pitch < -MaxPitch ? -MaxPitch : Pitch > MaxPitch ? MaxPitch : Pitch;
	Yaw += DeltaTime * deltaX * RotSpeedYaw;

	OrbitRadius += orbitRadiusDelta;
	
	// todo: ignore only GCToOrbit's yaw and pitch rotation and scale
	Cam->Transform = Matrix::CreateTranslation(Vector3(0.0f, 0.0f, OrbitRadius)) * 
		Matrix::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(Yaw), DirectX::XMConvertToRadians(Pitch), 0.0f) * 
		Matrix::CreateTranslation(GCToOrbit->GetWorldTransform().Position);
}
