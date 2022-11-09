#include "OrbitCameraController.h"

#include "InputDevice.h"
#include "SceneComponent.h"
#include "Game.h"


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
		Matrix::CreateTranslation(SCToOrbit->GetTransform().Position);

	Vector3 movementDelta;
	
	if (input.GetKeyboard()->IsDown(87))
	{
		movementDelta.x += DeltaTime * characterSpeed;
	}
	if (input.GetKeyboard()->IsDown(83))
	{
		movementDelta.x -= DeltaTime * characterSpeed;
	}
	if (input.GetKeyboard()->IsDown(68))
	{
		movementDelta.y += DeltaTime * characterSpeed;
	}
	if (input.GetKeyboard()->IsDown(65))
	{
		movementDelta.y -= DeltaTime * characterSpeed;
	}
	const float radius = 1.0f;
	const float rotAngleX = -movementDelta.x / radius;
	const float rotAngleY = -movementDelta.y / radius;
	Transform tempTransform = SCToOrbit->GetTransform();
	const Vector3 right = Cam->Transform.Rotation.GetRightVector();
	//tempTransform.Rotation.RotateAroundAxis(right, rotAngleX);
	const Vector3 forward = right.Cross(Vector3::Up);
	//tempTransform.Rotation.RotateAroundAxis(forward, rotAngleY);
	tempTransform.Position -= forward * movementDelta.x;
	tempTransform.Position += right * movementDelta.y;
	SCToOrbit->SetTransform(tempTransform);
}
