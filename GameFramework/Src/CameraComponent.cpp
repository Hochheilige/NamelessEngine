#include "CameraComponent.h"

#include "Game.h"
#include "DisplayWin32.h"
#include <InputDevice.h>

CameraComponent::CameraComponent()
{
	povData.UpdateProjectionMatrixPerspective(45.0f, Game::GetInstance()->GetDisplay()->GetAspectRatio(), 0.01f, 1000.0f);
	povData.Transform.Position = { -3.0f, 12.0f, 9.0f };
	SetTransform(povData.Transform);
	Game::GetInstance()->SetPlayerCamera(this);
}


auto CameraComponent::GetPOVData() const -> const Camera&
{
	// TODO: find a proper place to keep transform synchronised?
	povData.Transform = GetTransform();

	return povData;
}

auto CameraComponent::UpdateAspectRatio(float newAspectRatio) -> void
{
	povData.UpdateAspectRatio(newAspectRatio);
}

auto CameraComponent::GetMouseDeltaX() -> float
{
	InputDevice& input = *Game::GetInstance()->GetInputDevice();
	
	float deltaX;
	float deltaY;
	input.GetMouse()->GetDeltas(deltaX, deltaY);

	return deltaX;
}

auto CameraComponent::GetMouseDeltaY() -> float
{
	InputDevice& input = *Game::GetInstance()->GetInputDevice();

	float deltaX;
	float deltaY;
	input.GetMouse()->GetDeltas(deltaX, deltaY);

	return deltaY;
}
