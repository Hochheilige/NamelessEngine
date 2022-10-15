#include "CSBind.h"
#include "Sandbox.h"

void CSBind::CS_CreateObj(float trans_x, float trans_y, float trans_z, float rot_x, float rot_y, float rot_z, float scale_x, float scale_y, float scale_z)
{
	auto instance = (Sandbox*)Sandbox::GetInstance();
	instance->CreateCubeObject( trans_x,  trans_y,  trans_z,  rot_x,  rot_y,  rot_z,  scale_x,  scale_y,  scale_z);
}

void CSBind::CS_AudioOnCreate()
{
	std::cout << "AudioComponent Registered" << std::endl;
}
