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

	class RenderPrimitiveProxy* boxMeshProxy;
	class RenderPrimitiveProxy* circleMeshProxy;
	class RenderPrimitiveProxy* sphereMeshProxy;

	class Camera* PerspCamera;
	class Camera* OrthoCamera;
	class OrbitCameraController* OrbitCC;
	class CameraController* FPSCC;
	class GameComponent* CurrentCC = nullptr;
};