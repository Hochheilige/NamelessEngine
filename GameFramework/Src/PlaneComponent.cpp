#include "PlaneComponent.h"

#include "Shader.h"

#include <vector>

using namespace std;

PlaneComponent::PlaneComponent(std::shared_ptr<Transform> transform) : LineRenderer(transform)
{	
	struct ConstrStatic
	{
		ComPtr<ID3D11Buffer> VertexBuffer;
		int numVerts;

		ConstrStatic()
		{
			const int gridSize = 20;
			const float distBetweenLines = 4.0f;
			const float farthestCoord = gridSize / 2.0f * distBetweenLines;
			std::vector<Vector3> points;
			for (int i = 0; i <= gridSize; ++i)
			{
				// Add vertical line
				points.push_back(Vector3(i * distBetweenLines - farthestCoord, 0.0f, -farthestCoord));
				points.push_back(Vector3(i * distBetweenLines - farthestCoord, 0.0f, farthestCoord));
			}

			// Add horizontal lines
			for (int i = 0; i <= gridSize; ++i)
			{
				// add vertical line
				points.push_back(Vector3(-farthestCoord, 0.0f, i * distBetweenLines - farthestCoord));
				points.push_back(Vector3(farthestCoord, 0.0f, i * distBetweenLines - farthestCoord));
			}

			D3D11_BUFFER_DESC vertexBufDesc = {};
			vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufDesc.CPUAccessFlags = 0;
			vertexBufDesc.MiscFlags = 0;
			vertexBufDesc.StructureByteStride = 0;
			vertexBufDesc.ByteWidth = sizeof(points[0]) * points.size();

			D3D11_SUBRESOURCE_DATA vertexData = {};
			vertexData.pSysMem = &points[0];
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();
			device->CreateBuffer(&vertexBufDesc, &vertexData, VertexBuffer.GetAddressOf());

			numVerts = points.size();
		}
	};
	
	static ConstrStatic constrStatic;

	VertexBuffer = constrStatic.VertexBuffer;
	numVerts = constrStatic.numVerts;
}

