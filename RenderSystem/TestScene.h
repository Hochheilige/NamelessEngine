#pragma once

#include "Graphics.h"
#include "Scene.h"
#include "GameComponent.h"


class TestScene : public Scene
{
public:
	virtual void Initialize() override;

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