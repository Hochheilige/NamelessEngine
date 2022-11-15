#pragma once

#include <d3d11.h>

#include <wrl/client.h>
using namespace Microsoft::WRL;

class RenderPrimitiveProxy
{
public:

	void Initialize(void* InVertexData, UINT InNumVertices, UINT InVertexSize, UINT* InIndexData, UINT InNumIndices);

	// todo: implement these functions and possibly make IndexedMeshProxy a separate class
	///*
	// * Sets own vertex and index buffers on the context
	// */
	//void SetBuffersOnContext(ComPtr<class ID3D11DeviceContext> Context);
	///*
	// * Uses appropriate draw call draw self (might use draw indexed or just draw)
	// */
	//void DrawSelfOnContext(ComPtr<class ID3D11DeviceContext> Context);

	ComPtr<ID3D11Buffer> GetVertexBuffer();
	ComPtr<ID3D11Buffer> GetIndexBuffer();

	UINT* GetStrides();
	UINT* GetOffsets();

	UINT GetNumIndices();

protected:

	ComPtr<ID3D11Buffer> VertexBuffer;
	ComPtr<ID3D11Buffer> IndexBuffer;

	UINT VertexSize = 0;

	UINT NumIndices = 0;

};