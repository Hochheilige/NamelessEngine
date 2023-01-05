#pragma once

#include "Renderer.h"
#include "RenderingSystemTypes.h"
#include "MonoObjects/StaticMeshRendererComponent.h"
#include <d3d11.h>
#include <filesystem>
using Path = std::filesystem::path;

#include <wrl/client.h>
using namespace Microsoft::WRL;

class StaticMesh;

class StaticMeshRenderer : public Renderer
{
public: 

	friend class ImGuiSubsystem;

	StaticMeshRenderer();

	auto GetStaticMesh() const -> StaticMesh* { return staticMesh; }
	auto SetStaticMesh(StaticMesh* inStaticMesh) -> void { staticMesh = inStaticMesh; }

	virtual auto Render(const RenderingSystemContext& RSContext) -> void override;

	auto SetMeshPath(std::string meshPath) -> void;
	auto SetTexturePath(std::string texturePath) -> void;

	ComponentType GetComponentType() override { return StaticMeshRendererType; }
	MonoComponent* GetMonoComponent() override { return mMonoComponent; }

	// todo: remove this
	LitMaterial Mat;
	void SetAlbedoSRV(ComPtr<ID3D11ShaderResourceView> InAlbedoSRV) { mAlbedoSRV = InAlbedoSRV; }
	void SetNormalSRV(ComPtr<ID3D11ShaderResourceView> InSRV) { mNormalSRV = InSRV; }
	void SetSpecularSRV(ComPtr<ID3D11ShaderResourceView> InSRV) { mSpecularSRV = InSRV; }

	json Serialize() const override;
	void Deserialize(const json* in) override;
	static Component* Create()
	{
		return new StaticMeshRenderer();
	}

	auto GetTexturePath() -> Path { return texturePath; }

protected:
	MonoComponent* mMonoComponent = new StaticMeshRendererComponent();

	// The mesh to render
	StaticMesh* staticMesh = nullptr;

	ComPtr<ID3D11ShaderResourceView> mAlbedoSRV = nullptr;
	ComPtr<ID3D11ShaderResourceView> mNormalSRV = nullptr;
	ComPtr<ID3D11ShaderResourceView> mSpecularSRV = nullptr;

private:
	std::string texturePath;
};