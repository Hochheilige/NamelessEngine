#include "GBuffer.h"

#include <d3d11.h>

#include "Game.h"


GBuffer::GBuffer(int Width, int Height)
{
	Resize(Width, Height);
}

void GBuffer::Resize(int Width, int Height)
{
	Dispose();

	ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();


#pragma region Create textures

	D3D11_TEXTURE2D_DESC texDesc =
	{
		static_cast<UINT>(Width),
		static_cast<UINT>(Height),
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_SAMPLE_DESC {1, 0},
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		0,
		0
	};

	device->CreateTexture2D(&texDesc, nullptr, &DiffuseTex);

	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	device->CreateTexture2D(&texDesc, nullptr, &NormalTex);
	device->CreateTexture2D(&texDesc, nullptr, &WorldPositionTex);

#pragma endregion Create textures

	device->CreateShaderResourceView(DiffuseTex.Get(), nullptr, &DiffuseSRV);
	device->CreateShaderResourceView(NormalTex.Get(), nullptr, &NormalSRV);
	device->CreateShaderResourceView(WorldPositionTex.Get(), nullptr, &WorldPositionSRV);

	device->CreateRenderTargetView(DiffuseTex.Get(), nullptr, &DiffuseRTV);
	device->CreateRenderTargetView(NormalTex.Get(), nullptr, &NormalRTV);
	device->CreateRenderTargetView(WorldPositionTex.Get(), nullptr, &WorldPositionRTV);
}

void GBuffer::Dispose()
{
	DiffuseTex.Reset();
	DiffuseSRV.Reset();
	DiffuseRTV.Reset();
	
	NormalTex.Reset();
	NormalSRV.Reset();
	NormalRTV.Reset();
		
	WorldPositionTex.Reset();
	WorldPositionSRV.Reset();
	WorldPositionRTV.Reset();
}
