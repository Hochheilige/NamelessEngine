#pragma once

#include "Renderer.h"
#include "Game.h"

#include "RenderingSystemTypes.h"

#include "RenderingSystem.h"
#include "MonoObjects/MeshRendererComponent.h"

class MeshRenderer : public Renderer
{
public:

	MeshRenderer(bool ShouldRegister = true);

	virtual void Init() override {}
	virtual void OnRegister() override {}
	virtual void Update(float DeltaTime) override;

	void SetMeshProxy(class MeshProxy* InMeshProxy);

	void SetAlbedoSRV(ComPtr<ID3D11ShaderResourceView> InAlbedoSRV);
	void SetNormalSRV(ComPtr<ID3D11ShaderResourceView> InSRV) { mNormalSRV = InSRV; }
	void SetSpecularSRV(ComPtr<ID3D11ShaderResourceView> InSRV) { mSpecularSRV = InSRV; }

	virtual void Render(const RenderingSystemContext& RSContext) override;

	ComponentType GetComponentType() override { return mType; }

	MonoComponent* GetMonoComponent() override { return mMonoComponent; }
	
	LitMaterial Mat;

protected:
	ComponentType mType = ComponentType::MeshRendererType;
	MonoComponent* mMonoComponent = new MeshRendererComponent;
	
	class MeshProxy* mMeshProxy = nullptr;

	ComPtr<ID3D11ShaderResourceView> mAlbedoSRV = nullptr;
	ComPtr<ID3D11ShaderResourceView> mNormalSRV = nullptr;
	ComPtr<ID3D11ShaderResourceView> mSpecularSRV = nullptr;
};

