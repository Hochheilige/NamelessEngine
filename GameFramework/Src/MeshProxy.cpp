#include "MeshProxy.h"

#include "Game.h"

void MeshProxy::Initialize(void* InVertexData, UINT InNumVertices, UINT InVertexSize, UINT* InIndexData, UINT InNumIndices)
{
	NumIndices = InNumIndices;
	VertexSize = InVertexSize;

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = InNumVertices * InVertexSize;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = InVertexData;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();
	device->CreateBuffer(&vertexBufDesc, &vertexData, VertexBuffer.GetAddressOf());

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(InIndexData[0]) * static_cast<UINT>(NumIndices);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = InIndexData;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&indexBufDesc, &indexData, IndexBuffer.GetAddressOf());
}

ComPtr<ID3D11Buffer> MeshProxy::GetVertexBuffer()
{
	return VertexBuffer;
}

ComPtr<ID3D11Buffer> MeshProxy::GetIndexBuffer()
{
	return IndexBuffer;
}

UINT* MeshProxy::GetStrides()
{
	return &VertexSize;
}

UINT* MeshProxy::GetOffsets()
{
	static UINT offset = 0;
	return &offset;
}

UINT MeshProxy::GetNumIndices()
{
	return NumIndices;
}
