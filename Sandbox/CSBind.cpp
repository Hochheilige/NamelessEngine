#include "CSBind.h"
#include "Sandbox.h"

void CSBind::CS_CreateObj(float trans_x, float trans_y, float trans_z, float rot_x, float rot_y, float rot_z, float scale_x, float scale_y, float scale_z)
{
	auto instance = (Sandbox*) Sandbox::GetInstance();
	instance->CreateCubeObject(-2.0f, -4.0f, trans_z, 0, 0, 0, 1.0f, 1.0f, 1.0f);
}
