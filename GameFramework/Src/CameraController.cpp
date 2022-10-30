#include "CameraController.h"

#include "Camera.h"
#include "Game.h"
#include "InputDevice.h"
#include "Keyboard.h"

CameraController::CameraController()
{
	bShouldUpdate = true;
}

void CameraController::SetCameraToControl(Camera* InCamera)
{
	Cam = InCamera;
	if (Cam)
	{
		FOV = Cam->GetFOV();
		minFov = FOV;
	}
}

void CameraController::Update(float DeltaTime)
{
	if (Cam == nullptr)
	{
		return;
	}
	InputDevice& input = *Game::GetInstance()->GetInputDevice();

	Vector3 movementDelta = Vector3::Zero;

	auto keyboard = input.GetKeyboard();

	if (keyboard->IsDown(KEY_W))
	{
		movementDelta.x += DeltaTime * Speed;
	}
	if (keyboard->IsDown(KEY_S))
	{
		movementDelta.x -= DeltaTime * Speed;
	}
	if (keyboard->IsDown(KEY_D))
	{
		movementDelta.y += DeltaTime * Speed;
	}
	if (keyboard->IsDown(KEY_A))
	{
		movementDelta.y -= DeltaTime * Speed;
	}
	if (keyboard->IsDown(KEY_E))
	{
		movementDelta.z += DeltaTime * Speed;
	}
	if (keyboard->IsDown(KEY_Q))
	{
		movementDelta.z -= DeltaTime * Speed;
	}

	if (keyboard->IsDown(KEY_C))
	{
		FOV += DeltaTime * FOVChangeSpeed;
	}
	else
	{
		FOV -= DeltaTime * FOVChangeSpeed;
	}

	FOV = FOV < minFov ? minFov : (FOV > maxFov ? maxFov : FOV);

	Cam->UpdateFOV(FOV);

	Cam->Transform.Position += Cam->Transform.Rotation.GetForwardVector() * movementDelta.x
		+ Cam->Transform.Rotation.GetRightVector() * movementDelta.y
		+ Vector3::Up * movementDelta.z;

	float deltaX;
	float deltaY;
	input.GetMouse()->GetDeltas(deltaX, deltaY);

	if (input.GetMouse()->IsDown(RIGHT)){
		Pitch += DeltaTime * deltaY * RotSpeedPitch;
		Pitch = Pitch < -MaxPitch ? -MaxPitch : Pitch > MaxPitch ? MaxPitch : Pitch;
		Yaw += DeltaTime * deltaX * RotSpeedYaw;
	}

	Cam->Transform.Rotation.SetEulerAngles(Pitch, Yaw, 0.0f);
}
