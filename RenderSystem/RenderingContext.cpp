#include "RenderingContext.h"

#include "Graphics.h"
#include "Shader.h"

RenderingContext::RenderingContext()
{
	Graphics* gf = Graphics::GetInstance();
	ComPtr<ID3D11Device> device = gf->GetDevice();

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

	D3D11_RASTERIZER_DESC rastDesc{};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	device->CreateRasterizerState(&rastDesc, &CullBackRasterizerState);

	rastDesc.CullMode = D3D11_CULL_FRONT;

	device->CreateRasterizerState(&rastDesc, &CullFrontRasterizerState);

	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	// Col = Src * 1 + Dest * 0
	// Alpha = SrcAlpha * 1 + DestAlpha * 0
	device->CreateBlendState(&blendDesc, &OpaqueBlendState);

	D3D11_BUFFER_DESC bufDesc{};
	bufDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	bufDesc.MiscFlags = 0;
	bufDesc.StructureByteStride = 0;
	bufDesc.ByteWidth = sizeof(CBPerDraw);

	device->CreateBuffer(&bufDesc, nullptr, &PerDrawCB);

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

}

void RenderingContext::Initialize(Scene* current_scene)
{
	scene = current_scene;

	Graphics* gf = Graphics::GetInstance();

	// @TODO: Move context to rendering system?
	ID3D11DeviceContext* context = gf->GetContext().Get();

	context->RSSetState(CullBackRasterizerState.Get());
	context->OMSetBlendState(OpaqueBlendState.Get(), nullptr, D3D11_DEFAULT_SAMPLE_MASK);
	context->OMSetDepthStencilState(OpaqueDepthStencilState.Get(), 0);

	// @TODO: move to function for reuse?
	/////////////////////////////////////////////////////////////////////////////////
	context->VSSetConstantBuffers(0, 1, PerDrawCB.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, PerDrawCB.GetAddressOf());

	CBPerDraw cbData;
	const Camera& cam = gf->GetCurrentCamera();
	cbData.WorldToClip = cam.GetWorldToClipMatrix();
	cbData.CameraWorldPos = cam.Transform.Position;
	cbData.ViewToClip = cam.GetProjectionMatrix();
	cbData.WorldToView = cam.GetViewMatrix();

	D3D11_MAPPED_SUBRESOURCE resource = {};
	auto res = context->Map(PerDrawCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, &cbData, sizeof(cbData));

	context->Unmap(PerDrawCB.Get(), 0);
	/////////////////////////////////////////////////////////////////////////////////

	context->PSSetSamplers(0, 1, DefaultSampler.GetAddressOf());

	SetScreenSizeViewport();

	context->ClearRenderTargetView(gf->GetRTV().Get(), Color(0.0f, 0.4f, 0.0f, 1.0f));
	context->ClearDepthStencilView(gf->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// @TODO: Move render target to variable
	ID3D11RenderTargetView* views[8] = { gf->GetRTV().Get(), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	// @TODO: move depth stencil view to rendering system
	context->OMSetRenderTargets(8, views, gf->GetDSV().Get());


}

void RenderingContext::DrawRenderers()
{
	RenderingSystemContext rsContext;
	rsContext.ShaderFlags = static_cast<int>(ShaderFlag::ForwardRendering | ShaderFlag::DirectionalLight);

	for (Renderer* renderer : scene->GetRenderers())
	{
		renderer->Render(rsContext);
	}
}

void RenderingContext::SetScreenSizeViewport()
{
	Graphics* gf = Graphics::GetInstance();
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(gf->GetScreenWidth());
	viewport.Height = static_cast<float>(gf->GetScreenHeight());
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	gf->GetContext()->RSSetViewports(1, &viewport);
}
