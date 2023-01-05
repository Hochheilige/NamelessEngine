#pragma once

#include "Asset.h"
#include <WICTextureLoader.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;


class NormalTexture : public Asset
{
	friend class AssetManager;
public:
	auto GetSRV() -> ComPtr<ID3D11ShaderResourceView> { return TexSRV; }

	virtual auto Load() -> bool override;

private:
	ComPtr<ID3D11Resource> Tex;
	ComPtr<ID3D11ShaderResourceView> TexSRV;
};