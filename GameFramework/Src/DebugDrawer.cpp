#include "DebugDrawer.h"

#include "Game.h"
#include "Shader.h"
#include "EngineContentRegistry.h"
#include "RenderingSystemTypes.h"

DebugDrawer::DebugDrawer()
{
	
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	Color col{ color.x(), color.y(), color.z(), 1.0f};
	renderBuffer.push_back({ {from.x(), from.y(), from.z()}, col});
	renderBuffer.push_back({ {to.x(), to.y(), to.z()}, col});
}

int DebugDrawer::getDebugMode() const
{
	return debugMode;
}

void DebugDrawer::setDebugMode(int debugMode)
{
	// todo: exclude unsupported modes such as drawing 3d text

	this->debugMode = debugMode;
}

auto DebugDrawer::Render() -> void
{
	if (renderBuffer.size() == 0)
	{
		return;
	}

	UpdateVertexBuffer();

	Game* game = Game::GetInstance();

	ComPtr<ID3D11DeviceContext> context = game->GetD3DDeviceContext();
	ComPtr<ID3D11SamplerState> defaultSamplerState = game->GetDefaultSamplerState();

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	EngineContentRegistry::GetInstance()->GetPosColorVertexShader()->UseShader();
	EngineContentRegistry::GetInstance()->GetPosColorPixelShader()->UseShader();

	UINT stride = sizeof(DebugVertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	CBPerObject cbData;
	cbData.ObjectToWorld = Matrix::Identity;

	D3D11_MAPPED_SUBRESOURCE resource = {};
	auto res = context->Map(game->GetPerObjectConstantBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, &cbData, sizeof(cbData));

	// context->PSSetConstantBuffers(2, 1, game->GetPerObjectConstantBuffer().GetAddressOf()); // prolly don't need that here
	context->VSSetConstantBuffers(2, 1, game->GetPerObjectConstantBuffer().GetAddressOf());

	context->Draw(renderBuffer.size(), 0);

	renderBuffer.clear();
}

auto DebugDrawer::UpdateVertexBuffer() -> void
{
	if (vertexBufferSizeInVertices < renderBuffer.size())
	{
		CreateVertexBuffer();
	}

	Game* game = Game::GetInstance();
	ComPtr<ID3D11DeviceContext> context = game->GetD3DDeviceContext();

	D3D11_MAPPED_SUBRESOURCE resource = {};
	auto res = context->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, renderBuffer.data(), renderBuffer.size() * sizeof(renderBuffer[0]));

	context->Unmap(vertexBuffer.Get(), 0);
}

auto DebugDrawer::CreateVertexBuffer() -> void
{
	vertexBufferSizeInVertices = renderBuffer.size();

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = vertexBufferSizeInVertices * sizeof(*(renderBuffer.data()));

	ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();
	device->CreateBuffer(&vertexBufDesc, nullptr, &vertexBuffer);
}
