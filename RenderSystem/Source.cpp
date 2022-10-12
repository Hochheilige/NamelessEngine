#include "MathInclude.h"
#include "Application.h"
#include "TestScene.h"

int main()
{
	Application* app = new Application();
	Scene* scene = new TestScene();
	scene->Initialize();
	app->Initialize(scene);

	app->Run();
}