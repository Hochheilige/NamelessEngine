#include "..\Include\Camera.h"

Matrix Camera::GetViewMatrix() const
{
	return Transform.GetInverseTransformMatrixTransposed();
}

Matrix Camera::GetProjectionMatrix() const
{
	return ProjectionMatrix.Transpose();
}

void Camera::UpdateProjectionMatrixPerspective(float FOV, float AspectRatio, float NearPlane, float FarPlane)
{
	this->FOV = FOV;
	this->AspectRatio = AspectRatio;
	this->NearPlane = NearPlane;
	this->FarPlane = FarPlane;
	
	ProjectionMatrix = Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(FOV), AspectRatio, NearPlane, FarPlane);
}

void Camera::UpdateProjectionMatrixOrthographic(float Width, float Height, float NearPlane, float FarPlane)
{
	ProjectionMatrix = Matrix::CreateOrthographic(Width, Height, NearPlane, FarPlane);
}

void Camera::UpdateFOV(float NewFOV)
{
	FOV = NewFOV;
	ProjectionMatrix = Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(NewFOV), AspectRatio, NearPlane, FarPlane);
}

void Camera::UpdateAspectRatio(float NewAspectRatio)
{
	AspectRatio = NewAspectRatio;
	ProjectionMatrix = Matrix::CreatePerspectiveFieldOfView(FOV, AspectRatio, NearPlane, FarPlane);
}

void Camera::HandleViewportResize(int NewWidth, int NewHeight)
{
	// todo: sace camera type and update projection matrix accordingly
}

// @TODO: Rename the function to signify we get a transposed matrix? Same for other functions
Matrix Camera::GetWorldToClipMatrix() const
{
	return (GetProjectionMatrix() * GetViewMatrix());
}

float Camera::GetFOV() const
{
	return FOV;
}
