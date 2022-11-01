#include "ObjectLookupHelper.h"

#include "RenderingSystem.h"
#include "Renderer.h"
#include "Actor.h"
#include "Shader.h"
#include "ShaderCompiler.h"

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
