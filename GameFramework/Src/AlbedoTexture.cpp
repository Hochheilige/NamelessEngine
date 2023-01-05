#include "AlbedoTexture.h"
#include "Game.h"

auto AlbedoTexture::Load() -> bool
{
	DirectX::CreateWICTextureFromFile(Game::GetInstance()->GetD3DDevice().Get(), 
		GetFullPath().wstring().c_str(), Tex.GetAddressOf(), TexSRV.GetAddressOf());
	return TexSRV.GetAddressOf() != nullptr;
}
