#include "Application.h"

Application::Application()
{
	Graphics* gf = Graphics::GetInstance();
	gf->Initialize();
}

void Application::Initialize(Scene* scene)
{
	Context->Initialize(scene);
}

void Application::Run()
{

}