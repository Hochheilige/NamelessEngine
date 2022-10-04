#pragma once

#include <wrl/client.h>

using namespace Microsoft::WRL;

struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;

class GBuffer
{
public:

	GBuffer(int Width, int Height);

	void Resize(int Width, int Height);

	void Dispose();

public:
	// @TODO: return raw pointers because the GBuffer only should keep track of these?
	ID3D11ShaderResourceView* GetDiffuseSRV() const { return DiffuseSRV.Get(); };
	ID3D11ShaderResourceView* GetNormalSRV() const { return NormalSRV.Get(); };
	ID3D11ShaderResourceView* GetWorldPositionSRV() const { return WorldPositionSRV.Get(); };

	ID3D11RenderTargetView* GetDiffuseRTV() const { return DiffuseRTV.Get(); };
	ID3D11RenderTargetView* GetNormalRTV() const { return NormalRTV.Get(); };
	ID3D11RenderTargetView* GetWorldPositionRTV() const { return WorldPositionRTV.Get(); };

private:

	
	ComPtr<ID3D11Texture2D> DiffuseTex = nullptr;
	ComPtr<ID3D11ShaderResourceView> DiffuseSRV = nullptr;
	ComPtr<ID3D11RenderTargetView> DiffuseRTV = nullptr;

	ComPtr<ID3D11Texture2D> NormalTex = nullptr;
	ComPtr<ID3D11ShaderResourceView> NormalSRV = nullptr;
	ComPtr<ID3D11RenderTargetView> NormalRTV = nullptr;


	ComPtr<ID3D11Texture2D> WorldPositionTex = nullptr;
	ComPtr<ID3D11ShaderResourceView> WorldPositionSRV = nullptr;
	ComPtr<ID3D11RenderTargetView> WorldPositionRTV = nullptr;


};