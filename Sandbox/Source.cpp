#include "Sandbox.h"

#include <MeshRenderer.h>

int main()
{
	Sandbox* sandbox = new Sandbox();

	sandbox->Run();

	delete sandbox;
}