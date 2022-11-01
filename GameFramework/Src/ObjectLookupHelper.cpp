#include "ObjectLookupHelper.h"

#include "RenderingSystem.h"
#include "Renderer.h"
#include "Actor.h"
#include "Shader.h"
#include "ShaderCompiler.h"

#include "ImGuiSubsystem.h"

#include <d3d11.h>

ObjectLookupHelper::ObjectLookupHelper(RenderingSystem* InRenderingSystem)
	: MyRenderingSystem(InRenderingSystem)
	, Device(InRenderingSystem->MyGame->GetD3DDevice())
	, DeviceContext(InRenderingSystem->MyGame->GetD3DDeviceContext())
{
	HandleScreenResize(MyRenderingSystem->ViewportSize);

	D3D11_BUFFER_DESC bufDesc{};
	bufDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufDesc.MiscFlags = 0;
	bufDesc.StructureByteStride = 0;
	bufDesc.ByteWidth = sizeof(CBLookup);

	Device->CreateBuffer(&bufDesc, nullptr, &LookupCB);

	ShaderCompiler sc;
	sc.SetIsDebug(true);
	sc.SetEntryPoint("PSMain");
	sc.SetTarget("ps_5_0");
	sc.SetPathToShader(L"../Shaders/IntShader.hlsl");

	LookupShader = sc.CreateShader<PixelShader>();
}

auto ObjectLookupHelper::HandleScreenResize(const Vector2& NewSize) -> void
{
	ResizeViewport(static_cast<int>(NewSize.x), static_cast<int>(NewSize.y));
}

auto ObjectLookupHelper::ResizeViewport(int Width, int Height) -> void
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = static_cast<UINT>(Width);
	texDesc.Height = static_cast<UINT>(Height);
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32_UINT;
	texDesc.SampleDesc = DXGI_SAMPLE_DESC{ 1, 0 };
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET;// | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_TEXTURE2D_DESC stagingTexDesc = texDesc;
	stagingTexDesc.Width = 1;
	stagingTexDesc.Height = 1;
	stagingTexDesc.Usage = D3D11_USAGE_STAGING;
	stagingTexDesc.BindFlags = 0;
	stagingTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	Device->CreateTexture2D(&texDesc, nullptr, &RenderTex);
	Device->CreateRenderTargetView(RenderTex.Get(), nullptr, &RenderTexRTV);

	Device->CreateTexture2D(&stagingTexDesc, nullptr, &StagingTex);

	CreateDepthStagingTexture(Width, Height);
}

auto ObjectLookupHelper::CreateDepthStagingTexture(UINT Width, UINT Height) -> void
{
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = Width;
	descDepth.Height = Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_STAGING;
	descDepth.BindFlags = 0;
	descDepth.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	descDepth.MiscFlags = 0;

	Device->CreateTexture2D(&descDepth, NULL, &DepthStagingTex);
}

auto ObjectLookupHelper::Render() -> void
{
	MyRenderingSystem->SetScreenSizeViewport();

	ID3D11RenderTargetView* views[8] = { RenderTexRTV.Get(), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	// todo use a second depth stencil texture since we prolly need to render transparent objects here
	// for now I'll just reuse the existing buffer - mb it's enough
	DeviceContext->OMSetRenderTargets(8, views, MyRenderingSystem->DepthStencilView.Get());
	Color clearColor{ 0.0f, 0.0f, 0.0f, 0.0f };
	DeviceContext->ClearRenderTargetView(RenderTexRTV.Get(), clearColor);
	DeviceContext->ClearDepthStencilView(MyRenderingSystem->DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	DeviceContext->RSSetState(MyRenderingSystem->CullNoneRasterizerState.Get());
	DeviceContext->OMSetBlendState(MyRenderingSystem->OpaqueBlendState.Get(), nullptr, D3D11_DEFAULT_SAMPLE_MASK);
	DeviceContext->OMSetDepthStencilState(MyRenderingSystem->OpaqueDepthStencilState.Get(), 0);

	DeviceContext->VSSetConstantBuffers(0, 1, MyRenderingSystem->PerDrawCB.GetAddressOf());

	DeviceContext->PSSetConstantBuffers(3, 1, LookupCB.GetAddressOf());

	RenderingSystemContext rsContext;
	rsContext.ShaderFlags = static_cast<int>(ShaderFlag::ForwardRendering);
	rsContext.OverridePixelShader = LookupShader;

	uint32_t i = 0;
	for (Renderer* renderer : MyRenderingSystem->Renderers)
	{
		D3D11_MAPPED_SUBRESOURCE resource = {};
		auto res = DeviceContext->Map(LookupCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		CBLookup cb;
		cb.id = i + 1;
		cb._pad[0] = 0.0f;
		cb._pad[1] = 1.0f;
		cb._pad[2] = 2.0f;
		memcpy(resource.pData, &cb, sizeof(cb));
		DeviceContext->Unmap(LookupCB.Get(), 0);

		renderer->Render(rsContext);

		++i;
	}
}

auto ObjectLookupHelper::GetRendererUnderPosition(const Vector2& Pos)->Renderer*
{
	// todo: check viewport size bounds
	D3D11_BOX sourceRegion;
	sourceRegion.left = static_cast<UINT>(Pos.x);
	sourceRegion.right = sourceRegion.left + 1;
	sourceRegion.top = static_cast<UINT>(Pos.y);
	sourceRegion.bottom = sourceRegion.top + 1;
	sourceRegion.front = 0;
	sourceRegion.back = 1;
	DeviceContext->CopySubresourceRegion(StagingTex.Get(), 0, 0, 0, 0, RenderTex.Get(), 0, &sourceRegion);

	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	HRESULT res = DeviceContext->Map(StagingTex.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);

	// todo: check array bounds
	const uint32_t index = static_cast<uint32_t*>(mappedResource.pData)[0];
	DeviceContext->Unmap(StagingTex.Get(), 0);

	if (index != 0)
	{
		return MyRenderingSystem->Renderers[index - 1];
	}

	return nullptr;
}

auto ObjectLookupHelper::GetActorUnderPosition(const Vector2& Pos)->Actor*
{
	Renderer* renderer = GetRendererUnderPosition(Pos);
	if (renderer != nullptr)
	{
		return renderer->GetOwner();
	}
	return nullptr;
}

auto ObjectLookupHelper::GetWorldPositionUnerScreenPosition(const Vector2& Pos) -> Vector3
{
	UINT x = static_cast<UINT>(Pos.x);
	UINT y = static_cast<UINT>(Pos.y);

	// todo: check viewport size bounds
	DeviceContext->CopySubresourceRegion(DepthStagingTex.Get(), 0, 0, 0, 0, MyRenderingSystem->DepthStencilTex.Get(), 0, nullptr);

	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	HRESULT res = DeviceContext->Map(DepthStagingTex.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);

	struct DepthTexData
	{
		float depth;
		uint8_t stencil;
		uint8_t smthElse[3];
	};
	DepthTexData* row = reinterpret_cast<DepthTexData*>(static_cast<uint8_t*>(mappedResource.pData) + mappedResource.RowPitch * y);
	const float depth = row[x].depth;
	
	DeviceContext->Unmap(DepthStagingTex.Get(), 0);

	Vector3 ndc;
	ndc.x = static_cast<float>(x) / MyRenderingSystem->ViewportSize.x * 2.0f - 1.0f;
	ndc.y = static_cast<float>(y) / MyRenderingSystem->ViewportSize.y * 2.0f - 1.0f;
	ndc.y *= -1.0f;
	//ndc.z = depth * 2.0f - 1.0f; - apperently the depth we get is already from -1.0f to 1.0f
	ndc.z = depth;

	//ImGuiSubsystem::GetInstance()->AddMessageToDisplay("depth = " + std::to_string(depth));
	//ImGuiSubsystem::GetInstance()->AddMessageToDisplay("ndc = " + std::to_string(ndc.x) + ", " + std::to_string(ndc.y) + ", " + std::to_string(ndc.z));

	Camera* cam = MyRenderingSystem->MyGame->GetCurrentCamera();
	return NdcToWorld(ndc, cam->GetViewMatrix(), cam->GetProjectionMatrix());
}

auto ObjectLookupHelper::WorldToNdc(const Vector3& WorldPos, const Matrix& ViewMatrix, const Matrix& ProjMatrix) -> Vector3
{
	const Vector4 worldPos = { WorldPos.x, WorldPos.y, WorldPos.z, 1.0f };
	const Vector4 viewPos = DirectX::XMVector4Transform(worldPos, ViewMatrix);
	const Vector4 clipPos = DirectX::XMVector4Transform(viewPos, ProjMatrix);
	const Vector4 ndcPos = clipPos / clipPos.w;
	
	const Vector4 clipPos2 = DirectX::XMVector4Transform(worldPos, MyRenderingSystem->MyGame->GetCurrentCamera()->GetWorldToClipMatrixTransposed().Transpose());
	const Vector4 ndcPos2 = clipPos2 / clipPos2.w;
	return Vector3(ndcPos.x, ndcPos.y, ndcPos.z);
}

auto ObjectLookupHelper::NdcToWorld(const Vector3& NdcPos, const Matrix& ViewMatrix, const Matrix& ProjMatrix) -> Vector3
{
	const Vector4 ndcPos = { NdcPos.x, NdcPos.y, NdcPos.z, 1.0f };

	const Vector4 clipSpacePos = DirectX::XMVector4Transform(ndcPos, ProjMatrix.Invert());
	const Vector4 viewPos = clipSpacePos / clipSpacePos.w;

	const Vector4 worldPos = DirectX::XMVector4Transform(viewPos, ViewMatrix.Invert());

	return Vector3{ worldPos.x, worldPos.y, worldPos.z };
}
