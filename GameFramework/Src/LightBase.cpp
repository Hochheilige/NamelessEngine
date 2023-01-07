#include "LightBase.h"
#include "Game.h"
#include "RenderingSystem.h"

LightBase::LightBase()
{
	Game::GetInstance()->MyRenderingSystem->RegisterLight(this);
}

LightBase::~LightBase()
{
	Game::GetInstance()->MyRenderingSystem->UnregisterLight(this);
}
