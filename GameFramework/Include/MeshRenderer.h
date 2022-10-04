#pragma once

#include "Renderer.h"
#include "Game.h"

#include <DirectXMath.h>

#include "RenderingSystem.h"

class MeshRenderer : public Renderer
{
public:
	void SetMeshProxy(class MeshProxy* InMeshProxy);

	void SetAlbedoSRV(ComPtr<ID3D11ShaderResourceView> InAlbedoSRV);
	void SetNormalSRV(ComPtr<ID3D11ShaderResourceView> InSRV) { mNormalSRV = InSRV; }
	void SetSpecularSRV(ComPtr<ID3D11ShaderResourceView> InSRV) { mSpecularSRV = InSRV;; }

	virtual void Render(const RenderingSystemContext& RSContext) override;

	LitMaterial Mat;

protected:
	class MeshProxy* mMeshProxy = nullptr;

	ComPtr<ID3D11ShaderResourceView> mAlbedoSRV = nullptr;
	ComPtr<ID3D11ShaderResourceView> mNormalSRV = nullptr;
	ComPtr<ID3D11ShaderResourceView> mSpecularSRV = nullptr;
};

