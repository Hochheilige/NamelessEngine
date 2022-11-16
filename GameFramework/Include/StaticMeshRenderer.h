#pragma once

#include "Renderer.h"
#include "RenderingSystemTypes.h"
#include "MonoObjects/StaticMeshRendererComponent.h"
#include <d3d11.h>

#include <wrl/client.h>
using namespace Microsoft::WRL;

class StaticMesh;

class StaticMeshRenderer : public Renderer
{
public: 

	StaticMeshRenderer();

	auto GetStaticMesh() const -> StaticMesh* { return staticMesh; }
	auto SetStaticMesh(StaticMesh* inStaticMesh) -> void { staticMesh = inStaticMesh; }

	virtual auto Render(const RenderingSystemContext& RSContext) -> void override;

	ComponentType GetComponentType() override { return ComponentType::StaticMeshRendererType; }
	MonoComponent* GetMonoComponent() override { return mMonoComponent; }

	// todo: remove this
	LitMaterial Mat;
	void SetAlbedoSRV(ComPtr<ID3D11ShaderResourceView> InAlbedoSRV) { mAlbedoSRV = InAlbedoSRV; }
	void SetNormalSRV(ComPtr<ID3D11ShaderResourceView> InSRV) { mNormalSRV = InSRV; }
	void SetSpecularSRV(ComPtr<ID3D11ShaderResourceView> InSRV) { mSpecularSRV = InSRV; }

protected:
	MonoComponent* mMonoComponent = new StaticMeshRendererComponent();

	// The mesh to render
	StaticMesh* staticMesh = nullptr;

	ComPtr<ID3D11ShaderResourceView> mAlbedoSRV = nullptr;
	ComPtr<ID3D11ShaderResourceView> mNormalSRV = nullptr;
	ComPtr<ID3D11ShaderResourceView> mSpecularSRV = nullptr;
};