#include "Sandbox.h"

#include <MeshRenderer.h>

//#include <iostream>

int main()
{
	//int a;
	//std::cin >> a;

	Sandbox* sandbox = new Sandbox();

	sandbox->Run();

	delete sandbox;
}