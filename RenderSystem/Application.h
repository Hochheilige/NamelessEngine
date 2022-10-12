#pragma once

#include "Graphics.h"
#include "Scene.h"
#include "RenderingContext.h"

// need rename
class Application
{
public:

	Application();

	void Initialize(Scene* scene);

	void Run();

	void Update();

	void Render();

private:

	RenderingContext* Context = nullptr;

	class Camera* PerspCamera;
	class Camera* OrthoCamera;
	class OrbitCameraController* OrbitCC;
	class CameraController* FPSCC;
	class GameComponent* CurrentCC = nullptr;
};