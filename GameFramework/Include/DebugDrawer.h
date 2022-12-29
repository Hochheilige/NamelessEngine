#pragma once

#include "LinearMath/btIDebugDraw.h"

#include "MathInclude.h"

#include <vector>

#include <wrl/client.h>
using namespace Microsoft::WRL;

class RenderingSystemContext;
class VertexShader;
class PixelShader;

class DebugDrawer : public btIDebugDraw
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

	std::vector<DebugVertex> renderBuffer;

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	auto UpdateVertexBuffer() -> void;

	auto CreateVertexBuffer() -> void;

	ComPtr<ID3D11Buffer> vertexBuffer;
	UINT vertexBufferSizeInVertices = 0;
};
