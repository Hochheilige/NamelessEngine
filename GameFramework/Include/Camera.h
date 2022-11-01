#pragma once

#include "Transform.h"

// @TODO: make this a game component
class Camera
{
public:
	Transform Transform;

	Matrix GetViewMatrix() const;
	Matrix GetViewMatrixTransposed() const;

	Matrix GetProjectionMatrix() const;
	Matrix GetProjectionMatrixTransposed() const;

	void UpdateProjectionMatrixPerspective(float FOV, float AspectRatio, float NearPlane, float FarPlane);
	void UpdateProjectionMatrixOrthographic(float Width, float Height, float NearPlane, float FarPlane);

	void UpdateFOV(float NewFOV);

	void UpdateAspectRatio(float NewAspectRatio);

	void HandleViewportResize(int NewWidth, int NewHeight);

	Matrix GetWorldToClipMatrixTransposed() const;

	float GetFOV() const;
	
private:
	Matrix ProjectionMatrix = Matrix::Identity;

	float FOV = 20.0f;
	float AspectRatio = 1.0f;
	float NearPlane = 0.1f;
	float FarPlane = 100.0f;
};