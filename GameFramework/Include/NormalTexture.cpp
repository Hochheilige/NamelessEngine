#include "NormalTexture.h"
#include "Game.h"

auto NormalTexture::Load() -> bool
{
	DirectX::CreateWICTextureFromFileEx(
		Game::GetInstance()->GetD3DDevice().Get(),
		GetFullPath().wstring().c_str(),
		0,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0, 0,
		DirectX::WIC_LOADER_IGNORE_SRGB,
		Tex.GetAddressOf(),
		TexSRV.GetAddressOf());

	return TexSRV.GetAddressOf() != nullptr;
}
