#include "LineRenderer.h"
#include "Shader.h"

#include "RenderingSystem.h"

LineRenderer::LineRenderer()
{
	bCastShadow = false;
	Game::GetInstance()->MyRenderingSystem->RegisterRenderer(this);
}

void LineRenderer::Render(const RenderingSystemContext& RSContext)
{
	if (mVertexShader == nullptr || mPixelShader == nullptr || VertexBuffer.Get() == nullptr)
	{
		return;
	}

	Game* game = Game::GetInstance();

	ComPtr<ID3D11DeviceContext> context = game->GetD3DDeviceContext();

	const D3D_PRIMITIVE_TOPOLOGY topology = DrawAsStrip ? D3D_PRIMITIVE_TOPOLOGY_LINESTRIP : D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	context->IASetPrimitiveTopology(topology);

	// todo: optimize rendering by 
	// checking which shader is set now
	// and/or sorting meshes by used shaders
	mVertexShader->UseShader();
	
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

	D3D11_MAPPED_SUBRESOURCE resource = {};
	auto res = context->Map(game->GetPerObjectConstantBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, &cbData, sizeof(cbData));

	context->Unmap(game->GetPerObjectConstantBuffer().Get(), 0);

	UINT strides[] = { 12 };
	UINT offsets[] = { 0 };


	context->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), strides, offsets);

	context->PSSetConstantBuffers(2, 1, game->GetPerObjectConstantBuffer().GetAddressOf());
	context->VSSetConstantBuffers(2, 1, game->GetPerObjectConstantBuffer().GetAddressOf());

	context->Draw(numVerts, 0);
}
