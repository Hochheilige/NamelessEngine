#pragma once

#include "Renderer.h"
#include "Game.h"

struct RenderingSystemContext;

class LineRenderer : public Renderer
{
public:

	LineRenderer();

	virtual void Init() override {}
	virtual void OnRegister() override {}
	virtual void Update(float DeltaTime) override {}

	virtual void Render(const RenderingSystemContext& RSContext) override;

protected:

	ComPtr<ID3D11Buffer> VertexBuffer;

	bool DrawAsStrip = false;

	UINT numVerts = 0;
};

// todo: move to a separate file
using namespace DirectX;
class LineCircle : public LineRenderer
{
public:

	LineCircle() : LineRenderer()
	{
		struct ConstrStatic
		{
			ComPtr<ID3D11Buffer> VertexBuffer;
			UINT numVerts;

			ConstrStatic()
			{
				std::vector<Vector3> points;

				int numPoints = 20;
				const float deltaAlpha = XM_2PI / static_cast<float>(numPoints);
				for (int i = 0; i <= numPoints; ++i)
				{
					const float alpha = deltaAlpha * i;
					points.push_back(Vector3(cosf(alpha), 0.0f, sinf(alpha)));
				}

				D3D11_BUFFER_DESC vertexBufDesc = {};
				vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
				vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				vertexBufDesc.CPUAccessFlags = 0;
				vertexBufDesc.MiscFlags = 0;
				vertexBufDesc.StructureByteStride = 0;
				vertexBufDesc.ByteWidth = static_cast<UINT>(sizeof(points[0]) * points.size());

				D3D11_SUBRESOURCE_DATA vertexData = {};
				vertexData.pSysMem = &points[0];
				vertexData.SysMemPitch = 0;
				vertexData.SysMemSlicePitch = 0;

				ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();
				device->CreateBuffer(&vertexBufDesc, &vertexData, VertexBuffer.GetAddressOf());

				numVerts = static_cast<UINT>(points.size());
			}
		};

		static ConstrStatic constrStatic;

		VertexBuffer = constrStatic.VertexBuffer;
		numVerts = constrStatic.numVerts;

		DrawAsStrip = true;
	}

};

// todo: move to a separate file
class LineSphere : public LineRenderer
{
public:

	LineSphere() : LineRenderer()
	{
		struct ConstrStatic
		{
			ComPtr<ID3D11Buffer> VertexBuffer;
			UINT numVerts;

			ConstrStatic()
			{
				std::vector<Vector3> points;

				int numPoints = 50;
				const float deltaAlpha = XM_2PI / static_cast<float>(numPoints);
				for (int i = 0; i <= numPoints; ++i)
				{
					const float alpha = deltaAlpha * i - XM_PIDIV2;
					points.push_back(Vector3(cosf(alpha), 0.0f, sinf(alpha)));
				}
				for (int i = 0; i <= numPoints; ++i)
				{
					const float alpha = deltaAlpha * i - XM_PIDIV2;
					points.push_back(Vector3(0.0f, cosf(alpha), sinf(alpha)));
				}

				D3D11_BUFFER_DESC vertexBufDesc = {};
				vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
				vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				vertexBufDesc.CPUAccessFlags = 0;
				vertexBufDesc.MiscFlags = 0;
				vertexBufDesc.StructureByteStride = 0;
				vertexBufDesc.ByteWidth = static_cast<UINT>(sizeof(points[0]) * points.size());

				D3D11_SUBRESOURCE_DATA vertexData = {};
				vertexData.pSysMem = &points[0];
				vertexData.SysMemPitch = 0;
				vertexData.SysMemSlicePitch = 0;

				ComPtr<ID3D11Device> device = Game::GetInstance()->GetD3DDevice();
				device->CreateBuffer(&vertexBufDesc, &vertexData, VertexBuffer.GetAddressOf());

				numVerts = static_cast<UINT>(points.size());
			}
		};

		static ConstrStatic constrStatic;

		VertexBuffer = constrStatic.VertexBuffer;
		numVerts = constrStatic.numVerts;

		DrawAsStrip = true;
	}
};