#include "StaticMeshRenderer.h"

#include "AssetManager.h"
#include "Game.h"
#include "RenderingSystemTypes.h"
#include "Shader.h"
#include "StaticMesh.h"
#include "RenderingSystem.h"
#include "EngineContentRegistry.h"
#include "AlbedoTexture.h"
#include "NormalTexture.h"

StaticMeshRenderer::StaticMeshRenderer()
{
	// todo: move this out of constructor
	Game::GetInstance()->MyRenderingSystem->RegisterRenderer(this);
}

auto StaticMeshRenderer::Render(const RenderingSystemContext& RSContext) -> void
{
	if (mVertexShader == nullptr || mPixelShader == nullptr || staticMesh == nullptr || staticMesh->GetRenderData() == nullptr)
	{
		return;
	}

	Game* game = Game::GetInstance();

	ComPtr<ID3D11DeviceContext> context = game->GetD3DDeviceContext();
	ComPtr<ID3D11SamplerState> defaultSamplerState = game->GetDefaultSamplerState();

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mVertexShader->UseShader(static_cast<ShaderFlag>(RSContext.ShaderFlags));

	PixelShader* psToUse = RSContext.OverridePixelShader.value_or(mPixelShader);
	if (psToUse == nullptr)
	{
		context->PSSetShader(nullptr, nullptr, 0);
	}
	else
	{
		psToUse->UseShader(static_cast<ShaderFlag>(RSContext.ShaderFlags));
	}

	// Update constant buffer with world matrix
	CBPerObject cbData;
	cbData.ObjectToWorld = GetTransform().GetTransformMatrixTransposed();
	cbData.Color = mColor;
	cbData.NormalObjectToWorld = GetTransform().GetNormalMatrixTransposed();
	cbData.Mat = Mat;

	D3D11_MAPPED_SUBRESOURCE resource = {};
	auto res = context->Map(game->GetPerObjectConstantBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, &cbData, sizeof(cbData));

	context->Unmap(game->GetPerObjectConstantBuffer().Get(), 0);

	const StaticMeshRenderData* renderData = staticMesh->GetRenderData();

	context->IASetIndexBuffer(renderData->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	UINT offsets[] = {0};
	context->IASetVertexBuffers(0, 1, renderData->vertexBuffer.GetAddressOf(), &renderData->vertexSize, offsets);

	context->PSSetConstantBuffers(2, 1, game->GetPerObjectConstantBuffer().GetAddressOf());
	context->VSSetConstantBuffers(2, 1, game->GetPerObjectConstantBuffer().GetAddressOf());

	// Textures
	if (!(RSContext.ShaderFlags & static_cast<int>(ShaderFlag::DeferredLighting)))
		context->PSSetShaderResources(0, 1, mAlbedoSRV.GetAddressOf());
	context->PSSetSamplers(0, 1, defaultSamplerState.GetAddressOf());

	if (!(RSContext.ShaderFlags & static_cast<int>(ShaderFlag::DeferredLighting)))
		context->PSSetShaderResources(2, 1, mNormalSRV.GetAddressOf());

	if (!(RSContext.ShaderFlags & static_cast<int>(ShaderFlag::DeferredLighting)))
		context->PSSetShaderResources(3, 1, mSpecularSRV.GetAddressOf());

	// todo: render the scene with override material instead of using a bool
	if (!game->bIsRenderingShadowMap)
	{
		context->PSSetShaderResources(1, 1, game->GetShadowMapSRV().GetAddressOf());
		context->PSSetSamplers(1, 1, game->GetShadowmapSamplerState().GetAddressOf());
	}

	for (const StaticMeshSection& section : renderData->sections)
	{
		context->DrawIndexed(section.numIndices, section.indicesStart, section.vertexStart);
	}
}

auto StaticMeshRenderer::SetMeshPath(std::string meshPath) -> void
{
	SetStaticMesh(Game::GetInstance()->GetAssetManager()->LoadStaticMesh(meshPath));
	EngineContentRegistry* content = EngineContentRegistry::GetInstance();
	SetPixelShader(content->GetDefaultPixelShader());
	SetVertexShader(content->GetDefaultVertexShader());
	if (!mAlbedoSRV)
		SetAlbedoSRV(content->GetWhiteTexSRV());
	if (!mNormalSRV)
	SetNormalSRV(content->GetBasicNormalTexSRV());
}

auto StaticMeshRenderer::SetTexturePath(std::string texturePath) -> void
{
	if (texturePath == "") {
		SetAlbedoSRV(nullptr);
		return;
	}
	auto am = Game::GetInstance()->GetAssetManager();
	this->texturePath = texturePath;
	auto at = am->LoadAlbedoTexture(texturePath);
	if (at) {
		SetAlbedoSRV(at->GetSRV());
	}
	else
	{
		SetAlbedoSRV(nullptr);
	}
}

auto StaticMeshRenderer::SetNormalPath(std::string normalPath) -> void
{
	if (normalPath == "") {
		SetNormalSRV(nullptr);
		return;
	}
	auto am = Game::GetInstance()->GetAssetManager();
	this->normalPath = normalPath;
	auto at = am->LoadNormalTexture(normalPath);
	if (at) {
		SetNormalSRV(at->GetSRV());
	}
	else
	{
		SetNormalSRV(nullptr);
	}
}

json StaticMeshRenderer::Serialize() const
{
	auto out = Renderer::Serialize();
	out["mesh_path"] = GetStaticMesh() ? GetStaticMesh()->GetFullPath() : "";
	out["texture_path"] = texturePath;
	out["normal_path"] = normalPath;
	out["mat_ambient"] = Mat.ambientCoef;
	out["mat_diffuse"] = Mat.diffuesCoef;
	out["mat_specular"] = Mat.specularCoef;
	out["mat_specular_exp"] = Mat.specularExponent;
	return out;
}

void StaticMeshRenderer::Deserialize(const json* in)
{
	auto path = in->at("mesh_path").get<Path>();
	SetStaticMesh(Game::GetInstance()->GetAssetManager()->LoadStaticMesh(path));
	EngineContentRegistry* content = EngineContentRegistry::GetInstance();
	SetPixelShader(content->GetDefaultPixelShader());
	SetVertexShader(content->GetDefaultVertexShader());
	auto texPath = in->at("texture_path");
	SetTexturePath(texPath);
	if (mAlbedoSRV == nullptr) {
		SetAlbedoSRV(content->GetWhiteTexSRV());
	}
	texPath = in->at("normal_path");
	SetNormalPath(texPath);
	if (mNormalSRV == nullptr) {
		SetNormalSRV(content->GetBasicNormalTexSRV());
	}
	Mat.ambientCoef = in->at("mat_ambient");
	Mat.diffuesCoef = in->at("mat_diffuse");
	Mat.specularCoef = in->at("mat_specular");
	Mat.specularExponent = in->at("mat_specular_exp");
	Renderer::Deserialize(in);
}
