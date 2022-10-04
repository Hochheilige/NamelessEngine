#pragma once

#include "Transform.h"

// @TODO: make this a game component
class Camera
{
public:
	Transform Transform;

	Matrix GetViewMatrix() const;

	Matrix GetProjectionMatrix() const;

	void UpdateProjectionMatrixPerspective(float FOV, float AspectRatio, float NearPlane, float FarPlane);
	void UpdateProjectionMatrixOrthographic(float Width, float Height, float NearPlane, float FarPlane);

	void UpdateFOV(float NewFOV);

	Matrix GetWorldToClipMatrix() const;

	float GetFOV() const;
	
private:
	Matrix ProjectionMatrix = Matrix::Identity;

	float FOV;
	float AspectRatio;
	float NearPlane;
	float FarPlane;
};