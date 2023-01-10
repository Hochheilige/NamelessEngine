#pragma once

#include "LinearMath/btIDebugDraw.h"
#include "DebugDraw.h"

#include "MathInclude.h"

#include <vector>

#include <wrl/client.h>
using namespace Microsoft::WRL;

class RenderingSystemContext;
class VertexShader;
class PixelShader;

class DebugDrawer : public btIDebugDraw, public duDebugDraw
{
public:

	DebugDrawer();

	// Begin btIDebugDraw implementation
	// neccessary to override
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

	// neccessary to override
	virtual int getDebugMode() const override;

	virtual void setDebugMode(int debugMode) override;

	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override {};
	virtual void reportErrorWarning(const char* warningString) {};
	virtual void draw3dText(const btVector3& location, const char* textString) {};

	// End btIDebugDraw implementation

	void drawSolidTriangle(const btVector3& a, const btVector3& b, btVector3 c, const btVector3& color);

	// Begin duDebugDraw implementation
	virtual void depthMask(bool state);
	virtual void texture(bool state);
	virtual void begin(duDebugDrawPrimitives prim, float size = 1.0f);
	virtual void vertex(const float* pos, unsigned int color);
	virtual void vertex(const float x, const float y, const float z, unsigned int color);
	virtual void vertex(const float* pos, unsigned int color, const float* uv);
	virtual void vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v);
	virtual void end();
	// End duDebugDraw implementation

	void Render();

private:

	//int debugMode = DebugDrawModes::DBG_NoDebug;
	int debugMode = DebugDrawModes::DBG_DrawWireframe;

#pragma pack(push, 4)
	struct DebugVertex
	{
		Vector3 pos;
		Vector4 color;
	};
#pragma pack(pop)

	std::vector<DebugVertex> lineBuffer;

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	auto UpdateLineVertexBuffer() -> void;

	auto CreateLineVertexBuffer() -> void;

	ComPtr<ID3D11Buffer> vertexLineBuffer;
	UINT vertexLineBufferSizeInVertices = 0;

private:
	std::vector<DebugVertex> triangleBuffer;

	auto UpdateTriangleVertexBuffer() -> void;

	auto CreateTriangleVertexBuffer() -> void;

	// todo: use only one vertex buffer?
	ComPtr<ID3D11Buffer> triangleVertexBuffer;
	UINT vertexTriangleBufferSizeInVertices = 0;

private:
	duDebugDrawPrimitives curPrimType = static_cast<duDebugDrawPrimitives>(-1);
	int curPrimVertIndex = 0;
	btVector3 primInput[4];

	btVector3 navMeshColor{ 0.0f, 1.0f, 1.0f };
};
