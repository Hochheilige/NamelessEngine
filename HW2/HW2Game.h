#pragma once

#include "Game.h"

#include "GameComponent.h"

class RotateAroundActorComponent : public GameComponent
{
public:

	RotateAroundActorComponent()
	{
		bShouldUpdate = true;
	}

	GameComponent* GCToRotateAround = nullptr;

	float OrbitRadius = 2.0f;

	float RotAngle = 0.0f;

	// radians per second
	float RotSpeed = 0.6f;

	Vector3 Scale = Vector3::One;

	virtual void Update(float DeltaTime) override
	{
		if (GCToRotateAround != nullptr)
		{
			RotAngle += DeltaTime * RotSpeed;
			if (RotAngle > 6.28f)
			{
				RotAngle -= 6.28f;
			}

			const Transform parentTransform = GCToRotateAround->mTransform;
			Matrix mat = Matrix::CreateScale(Scale) * Matrix::CreateTranslation(Vector3(OrbitRadius, 0.0f, 0.0f)) *  Matrix::CreateFromAxisAngle(Vector3::Up, RotAngle) * parentTransform.GetTransformMatrix();
			this->GetParent()->mTransform.SetFromMatrix(mat);

		}
	}
};

class RotatingComponent : public GameComponent
{
public:
	RotatingComponent()
	{
		bShouldUpdate = true;
	}

	// radians per second
	float RotSpeed = 0.6f;

	

	virtual void Update(float DeltaTime) override
	{
		if (GetParent() != nullptr)
		{
			const float deltaAngle = DeltaTime * RotSpeed;
			const Vector3 upVec = GetParent()->mTransform.Rotation.GetUpVector();
			GetParent()->mTransform.Rotation.RotateAroundLocalAxis(Vector3::Up, deltaAngle);
		}
	}
};

class FloatingComponent : public GameComponent
{
public:

	FloatingComponent()
	{
		bShouldUpdate = true;
	}

	float Amplitude = 1.0f;
	float Phase = 0.0f;
	// In Units Per Second
	float Speed = 2.0f;


	virtual void Update(float DeltaTime) override
	{
		static float mult = 1.0f;
		if (GetParent() != nullptr)
		{
			const float time = Game::GetInstance()->GetTotalElapsedTime();
			GetParent()->mTransform.Position = Vector3(0.0f, sinf(Phase + time * Speed) * Amplitude, 0.0f);
			/*GetParent()->mTransform.Position.y += DeltaTime * Speed * mult;
			if (fabs(GetParent()->mTransform.Position.y) > Amplitude)
			{
				mult *= -1.0f;
			}*/
		}
	}
};

class HW2Game : public Game
{
public:
	virtual void PrepareResources() override;

	virtual void Update(float DeltaTime) override;

private:
	class BoxMesh* boxMesh;
	class CircleMesh* circleMesh;
	class SphereMesh* sphereMesh;
	class SimpleVertexShader* vs;
	class PixelShader* ps;
	class PixelShader* psPlain;

	class MeshProxy* boxMeshProxy;
	class MeshProxy* circleMeshProxy;
	class MeshProxy* sphereMeshProxy;

	class Camera* PerspCamera;
	class Camera* OrthoCamera;
	class OrbitCameraController* OrbitCC;
	class CameraController* FPSCC;
	class GameComponent* CurrentCC = nullptr;
};

