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
	lineBuffer.push_back({ {from.x(), from.y(), from.z()}, col});
	lineBuffer.push_back({ {to.x(), to.y(), to.z()}, col});
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
	if (lineBuffer.size() == 0 && triangleBuffer.size() == 0)
	{
		return;
	}

	UpdateLineVertexBuffer();
	UpdateTriangleVertexBuffer();

	Game* game = Game::GetInstance();

	ComPtr<ID3D11DeviceContext> context = game->GetD3DDeviceContext();
	ComPtr<ID3D11SamplerState> defaultSamplerState = game->GetDefaultSamplerState();

	EngineContentRegistry::GetInstance()->GetPosColorVertexShader()->UseShader();
	EngineContentRegistry::GetInstance()->GetPosColorPixelShader()->UseShader();

	CBPerObject cbData;
	cbData.ObjectToWorld = Matrix::Identity;

	// todo: use MapGuard to avoid issues with forgetting to unmap the resource in the future
	D3D11_MAPPED_SUBRESOURCE resource = {};
	auto res = context->Map(game->GetPerObjectConstantBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, &cbData, sizeof(cbData));

	context->Unmap(game->GetPerObjectConstantBuffer().Get(), 0);

	// context->PSSetConstantBuffers(2, 1, game->GetPerObjectConstantBuffer().GetAddressOf()); // prolly don't need that here
	context->VSSetConstantBuffers(2, 1, game->GetPerObjectConstantBuffer().GetAddressOf());


	UINT stride = sizeof(DebugVertex);
	UINT offset = 0;

	if (lineBuffer.size() > 0)
	{
		context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		context->IASetVertexBuffers(0, 1, vertexLineBuffer.GetAddressOf(), &stride, &offset);
		context->Draw(lineBuffer.size(), 0);
	}

	if (triangleBuffer.size() > 0)
	{
		context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->IASetVertexBuffers(0, 1, triangleVertexBuffer.GetAddressOf(), &stride, &offset);
		context->Draw(triangleBuffer.size(), 0);
	}

	lineBuffer.clear();
	triangleBuffer.clear();
}

auto DebugDrawer::UpdateLineVertexBuffer() -> void
{
	if (lineBuffer.size() == 0)
	{
		return;
	}

	if (vertexLineBufferSizeInVertices < lineBuffer.size())
	{
		CreateLineVertexBuffer();
	}

	Game* game = Game::GetInstance();
	ComPtr<ID3D11DeviceContext> context = game->GetD3DDeviceContext();

	D3D11_MAPPED_SUBRESOURCE resource = {};
	auto res = context->Map(vertexLineBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, lineBuffer.data(), lineBuffer.size() * sizeof(lineBuffer[0]));

	context->Unmap(vertexLineBuffer.Get(), 0);
}

auto DebugDrawer::CreateLineVertexBuffer() -> void
{
	vertexLineBufferSizeInVertices = lineBuffer.size();

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = vertexLineBufferSizeInVertices * sizeof(*(lineBuffer.data()));

	ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();
	device->CreateBuffer(&vertexBufDesc, nullptr, &vertexLineBuffer);
}

auto DebugDrawer::UpdateTriangleVertexBuffer() -> void
{
	if (triangleBuffer.size() == 0)
	{
		return;
	}

	if (vertexTriangleBufferSizeInVertices < triangleBuffer.size())
	{
		CreateTriangleVertexBuffer();
	}

	Game* game = Game::GetInstance();
	ComPtr<ID3D11DeviceContext> context = game->GetD3DDeviceContext();

	D3D11_MAPPED_SUBRESOURCE resource = {};
	auto res = context->Map(triangleVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	memcpy(resource.pData, triangleBuffer.data(), triangleBuffer.size() * sizeof(triangleBuffer[0]));

	context->Unmap(triangleVertexBuffer.Get(), 0);
}

auto DebugDrawer::CreateTriangleVertexBuffer() -> void
{
	vertexTriangleBufferSizeInVertices = triangleBuffer.size();

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = vertexTriangleBufferSizeInVertices * sizeof(*(triangleBuffer.data()));

	ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();
	device->CreateBuffer(&vertexBufDesc, nullptr, &triangleVertexBuffer);
}

void DebugDrawer::drawSolidTriangle(const btVector3& a, const btVector3& b, btVector3 c, const btVector3& color)
{
	Color col{ color.x(), color.y(), color.z(), 1.0f };
	triangleBuffer.push_back({ {a.x(), a.y(), a.z()}, col });
	triangleBuffer.push_back({ {b.x(), b.y(), b.z()}, col });
	triangleBuffer.push_back({ {c.x(), c.y(), c.z()}, col });
}

void DebugDrawer::depthMask(bool state)
{
	// todo: add a command to enable/disable writing to depth
	// this is also where we should enable alpha blending?
	// ignoring this for now
}

void DebugDrawer::texture(bool state)
{
	// todo: add a checker texture
	// ignoring this for now
}

void DebugDrawer::begin(duDebugDrawPrimitives prim, float size)
{
	// todo: implement size for points and lines - using geometry shader?
	// duDebugDrawPrimitives::DU_DRAW_POINTS : just draw points here 
	// duDebugDrawPrimitives::DU_DRAW_LINES : 2 vertices - one line
	// duDebugDrawPrimitives::DU_DRAW_TRIS : just the triangles; 3 vertices - one triangle?
	// duDebugDrawPrimitives::DU_DRAW_QUADS : two solid triangles
	curPrimVertIndex = 0;
	curPrimType = prim;
}

void DebugDrawer::vertex(const float* pos, unsigned int color)
{
	vertex(pos[0], pos[1], pos[2], color);
}

void DebugDrawer::vertex(const float x, const float y, const float z, unsigned int color)
{
	// ignoring color parameter for now

	primInput[curPrimVertIndex] = { x, y, z };

	switch (curPrimType)
	{
	case DU_DRAW_POINTS:
		// is this okay?
		drawLine(primInput[0], primInput[0], navMeshColor * 0.75f);
		break;
	case DU_DRAW_LINES:
		if (curPrimVertIndex == 1)
		{
			drawLine(primInput[0], primInput[1], navMeshColor * 0.6f);
		}
		break;
	case DU_DRAW_TRIS:
		if (curPrimVertIndex == 2)
		{
			drawSolidTriangle(primInput[0], primInput[1], primInput[2], navMeshColor);
		}
		break;
	case DU_DRAW_QUADS:
		if (curPrimVertIndex == 3)
		{
			drawSolidTriangle(primInput[0], primInput[1], primInput[2], navMeshColor);
			drawSolidTriangle(primInput[2], primInput[3], primInput[0], navMeshColor);
		}
		break;
	default:
		assert(false && "Current primitive type is not set - did you forget to call begin?");
		break;
	}

	curPrimVertIndex = (curPrimVertIndex + 1) % static_cast<int>(curPrimType + 1);
}

void DebugDrawer::vertex(const float* pos, unsigned int color, const float* uv)
{
	vertex(pos[0], pos[1], pos[2], color, uv[0], uv[1]);
}

void DebugDrawer::vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v)
{
	// ignoring color and uv parameters for now
	vertex(x, y, z, color);
}

void DebugDrawer::end()
{
	curPrimType = static_cast<duDebugDrawPrimitives>(-1);
}
