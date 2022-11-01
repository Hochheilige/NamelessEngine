#include <RenderingSystem.h>

#include "Game.h"
#include "Renderer.h"
#include "LightBase.h"
#include "Shader.h"
#include "ShaderCompiler.h"
#include "ObjectLookupHelper.h"

RenderingSystem::RenderingSystem(Game* InGame)
	: MyGame(InGame)
	, ViewportSize(InGame->GetScreenWidth(), InGame->GetScreenHeight())
	, GeometryBuffer(InGame->GetScreenWidth(), InGame->GetScreenHeight())
{
	ComPtr<ID3D11Device> device = MyGame->GetD3DDevice();

	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MaxLOD = INT_MAX;
	samplerDesc.MaxAnisotropy = 1.0f;

	device->CreateSamplerState(&samplerDesc, &DefaultSampler);

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;

	device->CreateSamplerState(&samplerDesc, &ShadowmapSampler);

	D3D11_RASTERIZER_DESC rastDesc{};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	device->CreateRasterizerState(&rastDesc, &CullBackRasterizerState);

	rastDesc.CullMode = D3D11_CULL_FRONT;

	device->CreateRasterizerState(&rastDesc, &CullFrontRasterizerState);

	rastDesc.CullMode = D3D11_CULL_NONE; 

	device->CreateRasterizerState(&rastDesc, &CullNoneRasterizerState);

	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	blendDesc.RenderTarget[0].BlendEnable = false;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	// Col = Src * 1 + Dest * 0
	// Alpha = SrcAlpha * 1 + DestAlpha * 0
	device->CreateBlendState(&blendDesc, &OpaqueBlendState);

	blendDesc.RenderTarget[0].BlendEnable = true;

	// Col = Src * SrcAlpha + Dest * 1
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	device->CreateBlendState(&blendDesc, &LightBlendState);

	D3D11_BUFFER_DESC bufDesc{};
	bufDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufDesc.MiscFlags = 0;
	bufDesc.StructureByteStride = 0;
	bufDesc.ByteWidth = sizeof(CBPerDraw);

	device->CreateBuffer(&bufDesc, nullptr, &PerDrawCB);

	bufDesc.ByteWidth = sizeof(CBPerObject);
	device->CreateBuffer(&bufDesc, nullptr, &PerObjectCB);

	bufDesc.ByteWidth = sizeof(CBLights);
	device->CreateBuffer(&bufDesc, nullptr, &LightsCB);

	// Compile shaders here ?


	// Subscribe to screen resized delegate

	D3D11_DEPTH_STENCIL_DESC depthDesc{};
	depthDesc.DepthEnable = true;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.StencilEnable = false;
	depthDesc.StencilReadMask = 0x00;
	depthDesc.StencilWriteMask = 0x00;

	device->CreateDepthStencilState(&depthDesc, &OpaqueDepthStencilState);

	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	device->CreateDepthStencilState(&depthDesc, &LightingLessDepthStencilState);

	depthDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	device->CreateDepthStencilState(&depthDesc, &LightingGreaterDepthStencilState);

	depthDesc.DepthEnable = false;
	device->CreateDepthStencilState(&depthDesc, &DisabledDepthStencilState);

	ResizeViewport(InGame->GetScreenWidth(), InGame->GetScreenHeight());

	MyObjectLookupHelper = new ObjectLookupHelper(this);
}

void RenderingSystem::RegisterRenderer(Renderer* InRenderer)
{
	// @TODO: add sorting by type
	if (InRenderer != nullptr)
	{
		Renderers.emplace_back(InRenderer);
	}
}

void RenderingSystem::UnregisterRenderer(Renderer* InRenderer)
{
	Renderers.erase(std::remove(Renderers.begin(), Renderers.end(), InRenderer));
}

void RenderingSystem::RegisterLight(LightBase* Light)
{
	if (Light != nullptr)
	{
		Lights.emplace_back(Light);
	}
}

void RenderingSystem::UnregisterLight(LightBase* Light)
{
	Lights.erase(std::remove(Lights.begin(), Lights.end(), Light));
}

void RenderingSystem::PerformShadowmapPass()
{
	ID3D11DeviceContext* context = MyGame->GetD3DDeviceContext().Get();

	// @TODO: setup rasterizer and blend states?

	// @TODO: remove this hack
	MyGame->bIsRenderingShadowMap = true;
	ID3D11ShaderResourceView* nullSRV = nullptr;
	context->PSSetShaderResources(1, 1, &nullSRV);
	context->OMSetRenderTargets(0, nullptr, MyGame->ShadowMapView.Get());

	D3D11_VIEWPORT viewport = {};
	viewport.Width = 2048;
	viewport.Height = 2048;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);

	context->ClearDepthStencilView(MyGame->ShadowMapView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	context->VSSetConstantBuffers(0, 1, PerDrawCB.GetAddressOf());
	CBPerDraw cbData;
	const Camera& cam = MyGame->LightCam;
	cbData.WorldToClip = cam.GetWorldToClipMatrixTransposed();

	D3D11_MAPPED_SUBRESOURCE resource = {};
	auto res = context->Map(PerDrawCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, &cbData, sizeof(cbData));

	context->Unmap(PerDrawCB.Get(), 0);

	RenderingSystemContext rsContext;
	rsContext.OverridePixelShader = nullptr;

	for (Renderer* renderer : Renderers)
	{
		if (renderer != nullptr && renderer->bCastShadow)
		{
			renderer->Render(rsContext);
		}
	}

	MyGame->bIsRenderingShadowMap = false;
}

void RenderingSystem::PerformForwardOpaquePass()
{
	// @TODO: Move context to rendering system?
	ID3D11DeviceContext* context = MyGame->GetD3DDeviceContext().Get();

	


	context->RSSetState(CullNoneRasterizerState.Get());
	context->OMSetBlendState(OpaqueBlendState.Get(), nullptr, D3D11_DEFAULT_SAMPLE_MASK);
	context->OMSetDepthStencilState(OpaqueDepthStencilState.Get(), 0);

	// @TODO: move to function for reuse?
	/////////////////////////////////////////////////////////////////////////////////
	context->VSSetConstantBuffers(0, 1, PerDrawCB.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, PerDrawCB.GetAddressOf());

	CBPerDraw cbData;
	const Camera& cam = *(MyGame->GetCurrentCamera());
	cbData.WorldToClip = cam.GetWorldToClipMatrixTransposed();
	cbData.CameraWorldPos = cam.Transform.Position;
	cbData.ViewToClip = cam.GetProjectionMatrixTransposed();
	cbData.WorldToView = cam.GetViewMatrixTransposed();

	D3D11_MAPPED_SUBRESOURCE resource = {};
	auto res = context->Map(PerDrawCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, &cbData, sizeof(cbData));

	context->Unmap(PerDrawCB.Get(), 0);
	/////////////////////////////////////////////////////////////////////////////////

	// Update lights buffer
	////////////////////////////////////////////////////////////
	context->VSSetConstantBuffers(1, 1, LightsCB.GetAddressOf());
	context->PSSetConstantBuffers(1, 1, LightsCB.GetAddressOf());

	CBLights lightsCBData;
	lightsCBData.LightData.Color = MyGame->DirectiLight.color;
	lightsCBData.LightData.Direction = MyGame->DirectiLight.direction;
	lightsCBData.LightData.Intensity = MyGame->DirectiLight.intensity;
	lightsCBData.LightData.WorldToLightClip = MyGame->LightCam.GetWorldToClipMatrixTransposed();
	D3D11_MAPPED_SUBRESOURCE lightsCBResource = {};
	res = context->Map(LightsCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightsCBResource);

	memcpy(lightsCBResource.pData, &lightsCBData, sizeof(lightsCBData));

	context->Unmap(LightsCB.Get(), 0);
	////////////////////////////////////////////////////////////

	context->PSSetSamplers(0, 1, DefaultSampler.GetAddressOf());
	context->PSSetSamplers(1, 1, ShadowmapSampler.GetAddressOf());

	SetScreenSizeViewport();
	ID3D11RenderTargetView* const target = ViewportRTV.Get();
	//ID3D11RenderTargetView* const target = MyGame->RenderTargetView.Get();
	context->ClearRenderTargetView(target, DiffuseClearColor);
	context->ClearDepthStencilView(DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	context->PSSetShaderResources(1, 1, MyGame->ShadowMapSRV.GetAddressOf());

	ID3D11RenderTargetView* views[8] = { target, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	context->OMSetRenderTargets(8, views, DepthStencilView.Get());

	RenderingSystemContext rsContext;
	rsContext.ShaderFlags = static_cast<int>(ShaderFlag::ForwardRendering | ShaderFlag::DirectionalLight);

	for (Renderer* renderer : Renderers)
	{
		renderer->Render(rsContext);
	}
}

void RenderingSystem::PerformOpaquePass(float DeltaTime)
{
	ID3D11DeviceContext* context = MyGame->GetD3DDeviceContext().Get();

	context->RSSetState(CullBackRasterizerState.Get());
	context->OMSetBlendState(OpaqueBlendState.Get(), nullptr, D3D11_DEFAULT_SAMPLE_MASK);
	context->OMSetDepthStencilState(OpaqueDepthStencilState.Get(), 0);

	

	// Shaders will be set from mesh renderer

	// Layout will be set from MeshRenderer (which will call the shader to set layout)

	// Per object contant buffers will be set by mesh renderer

	// @TODO: move to function for reuse?
	/////////////////////////////////////////////////////////////////////////////////
	context->VSSetConstantBuffers(0, 1, PerDrawCB.GetAddressOf());

	CBPerDraw cbData;
	const Camera& cam = *(MyGame->GetCurrentCamera());
	cbData.WorldToClip = cam.GetWorldToClipMatrixTransposed();
	cbData.CameraWorldPos = cam.Transform.Position;

	D3D11_MAPPED_SUBRESOURCE resource = {};
	auto res = context->Map(PerDrawCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, &cbData, sizeof(cbData));

	context->Unmap(PerDrawCB.Get(), 0);
	/////////////////////////////////////////////////////////////////////////////////


	context->PSSetSamplers(0, 1, DefaultSampler.GetAddressOf());
	context->PSSetSamplers(1, 1, ShadowmapSampler.GetAddressOf());

	SetScreenSizeViewport();

	context->ClearRenderTargetView(GeometryBuffer.GetDiffuseRTV(), DiffuseClearColor);
	context->ClearRenderTargetView(GeometryBuffer.GetNormalRTV(), Color(0.0f, 0.0f, 0.0f, 1.0f));
	context->ClearRenderTargetView(GeometryBuffer.GetWorldPositionRTV(), Color(0.0f, 0.0f, 0.0f, 1.0f));
	context->ClearDepthStencilView(DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	ID3D11RenderTargetView* views[8] = { GeometryBuffer.GetDiffuseRTV(), GeometryBuffer.GetNormalRTV(), GeometryBuffer.GetWorldPositionRTV(), nullptr, nullptr, nullptr, nullptr, nullptr };
	// @TODO: move depth stencil view to rendering system
	context->OMSetRenderTargets(8, views, DepthStencilView.Get());

	RenderingSystemContext rsContext;
	rsContext.ShaderFlags = static_cast<int>(ShaderFlag::DeferredOpaque);

	for (Renderer* renderer : Renderers)
	{
		renderer->Render(rsContext);
	}
}

void RenderingSystem::PerformLightingPass(float DeltaTime)
{
	ID3D11DeviceContext* context = MyGame->GetD3DDeviceContext().Get();

	context->OMSetBlendState(LightBlendState.Get(), nullptr, D3D11_DEFAULT_SAMPLE_MASK);

	context->PSSetConstantBuffers(0, 1, PerDrawCB.GetAddressOf());

	// @TODO: move to function for reuse? Add PSSet there too?
	/////////////////////////////////////////////////////////////////////////////////
	context->VSSetConstantBuffers(0, 1, PerDrawCB.GetAddressOf());

	CBPerDraw cbData;
	const Camera& cam = *(MyGame->GetCurrentCamera());
	cbData.WorldToClip = cam.GetWorldToClipMatrixTransposed();
	cbData.CameraWorldPos = cam.Transform.Position;;

	D3D11_MAPPED_SUBRESOURCE resource = {};
	auto res = context->Map(PerDrawCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, &cbData, sizeof(cbData));

	context->Unmap(PerDrawCB.Get(), 0);
	/////////////////////////////////////////////////////////////////////////////////

	// @TODO: do i need these samplers in this pass?
	context->PSSetSamplers(0, 1, DefaultSampler.GetAddressOf());
	context->PSSetSamplers(1, 0, ShadowmapSampler.GetAddressOf());

	SetScreenSizeViewport();

	context->ClearRenderTargetView(MyGame->RenderTargetView.Get(), Color(0.0f, 0.0f, 0.0f, 1.0f));

	// @TODO: pass a pointer to render target view to render to
	ID3D11RenderTargetView* views[8] = { MyGame->RenderTargetView.Get(), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	context->OMSetRenderTargets(8, views, DepthStencilView.Get());

	ID3D11ShaderResourceView* resources[] = {GeometryBuffer.GetDiffuseSRV(), MyGame->ShadowMapSRV.Get(), GeometryBuffer.GetNormalSRV(), GeometryBuffer.GetWorldPositionSRV()};
	context->PSSetShaderResources(0, sizeof(resources)/sizeof(resources[0]), resources);

	context->RSSetState(CullBackRasterizerState.Get());
	context->OMSetDepthStencilState(DisabledDepthStencilState.Get(), 0);

	// @TODO: set shadowmap here?

	// set light data here
	for (LightBase* light : Lights)
	{
		const LightType lightType = light->GetLightType();



		// @TODO: Move to function;
		//////////////////////////////////////////////////////////////
		context->VSSetConstantBuffers(1, 1, LightsCB.GetAddressOf());
		context->PSSetConstantBuffers(1, 1, LightsCB.GetAddressOf());
		CBLights lightsCBData;
		lightsCBData.LightData = light->GetLightData();
		// @TODO: pass lightspace matrix
		D3D11_MAPPED_SUBRESOURCE lightsCBResource = {};
		auto res = context->Map(LightsCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightsCBResource);

		memcpy(lightsCBResource.pData, &lightsCBData, sizeof(lightsCBData));

		context->Unmap(LightsCB.Get(), 0);
		//////////////////////////////////////////////////////////////


		RenderingSystemContext rsContext;
		rsContext.ShaderFlags = static_cast<int>(ShaderFlag::DeferredLighting | ShaderFlag::QuadOnly | ShaderFlag::AmbientLight);

		// @TODO: delete this
		if (lightType == LightType::Directional)
		{
			// Update lights buffer -why here?
			////////////////////////////////////////////////////////////
			context->VSSetConstantBuffers(1, 1, LightsCB.GetAddressOf());
			context->PSSetConstantBuffers(1, 1, LightsCB.GetAddressOf());

			CBLights lightsCBData;
			lightsCBData.LightData.Color = MyGame->DirectiLight.color;
			lightsCBData.LightData.Direction = MyGame->DirectiLight.direction;
			lightsCBData.LightData.Intensity = MyGame->DirectiLight.intensity;
			lightsCBData.LightData.WorldToLightClip = MyGame->LightCam.GetWorldToClipMatrixTransposed();
			D3D11_MAPPED_SUBRESOURCE lightsCBResource = {};
			res = context->Map(LightsCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lightsCBResource);

			memcpy(lightsCBResource.pData, &lightsCBData, sizeof(lightsCBData));

			context->Unmap(LightsCB.Get(), 0);
			rsContext.ShaderFlags = static_cast<int>(ShaderFlag::DeferredLighting | ShaderFlag::QuadOnly | ShaderFlag::DirectionalLight);
			////////////////////////////////////////////////////////////
		}


		switch (lightType)
		{
		case LightType::Ambient:
		case LightType::Directional:
			context->RSSetState(CullBackRasterizerState.Get());
			context->OMSetDepthStencilState(DisabledDepthStencilState.Get(), 0);
			
			// shaders, input layout, vertex buffer, index buffer will be set from renderer
			// @TODO: create quad renderer that will set index buffer to nullptr and vertex buffer too
			light->GetLightRenderer()->Render(rsContext);

			
			break;
		case LightType::Point:
		case LightType::Spot:
			// @TODO: add depth bounds check
			context->RSSetState(CullFrontRasterizerState.Get());
			context->OMSetDepthStencilState(DisabledDepthStencilState.Get(), 0);
			// @TODO: add shadows via shadow volumes?
			rsContext.ShaderFlags = static_cast<int>(ShaderFlag::DeferredLighting | ShaderFlag::PointLight);
			context->OMSetBlendState(LightBlendState.Get(), nullptr, D3D11_DEFAULT_SAMPLE_MASK);

			light->GetLightRenderer()->Render(rsContext);
			// shaders, input layout, vertex buffer, index buffer will be set from renderer

			// @TODO: add renderer to light and call draw on it
			break;
		}
	}
}

void RenderingSystem::HandleScreenResize(const Vector2& NewSize)
{
	if (NewSize.x < 1 || NewSize.y < 1)
	{
		return;
	}

	if (NewSize.x == ViewportSize.x && NewSize.y == ViewportSize.y)
	{
		return;
	}

	GeometryBuffer.Resize(NewSize.x, NewSize.y);
	ResizeViewport(NewSize.x, NewSize.y);
	MyObjectLookupHelper->HandleScreenResize(NewSize);
	MyGame->GetCurrentCamera()->UpdateAspectRatio(NewSize.x / NewSize.y);
}

auto RenderingSystem::GetActorUnderPosition(const Vector2& Pos) -> Actor*
{
	return MyObjectLookupHelper->GetActorUnderPosition(Pos);
}

void RenderingSystem::SetScreenSizeViewport()
{
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(ViewportSize.x);
	viewport.Height = static_cast<float>(ViewportSize.y);
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	MyGame->GetD3DDeviceContext()->RSSetViewports(1, &viewport);
}

void RenderingSystem::Draw(float DeltaTime, const Camera* InCamera)
{
	ComPtr<ID3D11DeviceContext> context = MyGame->GetD3DDeviceContext();

	context->ClearState();

	//PerformShadowmapPass();

	// @TODO: uncomment this
	//PerformOpaquePass(DeltaTime);
	//PerformLightingPass(DeltaTime);

	PerformForwardOpaquePass();

	MyObjectLookupHelper->Render();
}

void RenderingSystem::ResizeViewport(int Width, int Height)
{
	ViewportSize = { static_cast<float>(Width), static_cast<float>(Height) };
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

	ComPtr<ID3D11Device> device = MyGame->GetD3DDevice();

	device->CreateTexture2D(&texDesc, nullptr, &ViewportTex);
	device->CreateShaderResourceView(ViewportTex.Get(), nullptr, &ViewportSRV);
	device->CreateRenderTargetView(ViewportTex.Get(), nullptr, &ViewportRTV);

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = Width;
	descDepth.Height = Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	device->CreateTexture2D(&descDepth, NULL, &DepthStencilTex);

	// Create the depth stencil view
	device->CreateDepthStencilView(DepthStencilTex.Get(), // Depth stencil texture
		nullptr, // Depth stencil desc
		&DepthStencilView);  // [out] Depth stencil view

}
