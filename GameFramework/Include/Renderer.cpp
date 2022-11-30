#include "Renderer.h"

#include "Game.h"
#include "RenderingSystemTypes.h"
#include "Shader.h"
#include "RenderingSystem.h"

ComponentType Renderer::GetComponentType()
{
	return Undefined;
}

void QuadRenderer::Render(const RenderingSystemContext& RSContext)
{
	ID3D11DeviceContext* context = Game::GetInstance()->GetD3DDeviceContext().Get();

	context->IASetInputLayout(nullptr);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	ID3D11Buffer* nullBuffer = nullptr;
	UINT stride = 0;
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);
	context->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);

	mVertexShader->UseShader(static_cast<ShaderFlag>(RSContext.ShaderFlags));
	mPixelShader->UseShader(static_cast<ShaderFlag>(RSContext.ShaderFlags));

	context->Draw(4, 0);
}

Renderer::~Renderer()
{
	Game::GetInstance()->MyRenderingSystem->UnregisterRenderer(this);
}
