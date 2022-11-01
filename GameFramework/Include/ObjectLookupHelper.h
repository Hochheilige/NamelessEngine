#pragma once

#include <wrl/client.h>

#include "MathInclude.h"

class RenderingSystem;
class Renderer;
class Actor;
class PixelShader;

using namespace Microsoft::WRL;

struct CBLookup
{
	uint32_t id;
	float _pad[3];
};


class ObjectLookupHelper
{
public:

	ObjectLookupHelper(RenderingSystem* InRenderingSystem);

	auto HandleScreenResize(const Vector2& NewSize) -> void;

	auto GetRendererUnderPosition(const Vector2& Pos)->Renderer*;
	auto GetActorUnderPosition(const Vector2& Pos)->Actor*;

	auto GetWorldPositionUnerScreenPosition(const Vector2& Pos)->Vector3;

	auto Render() -> void;

private:
	auto ResizeViewport(int Width, int Height) -> void;

	auto CreateDepthStagingTexture(UINT Width, UINT Height) -> void;

	auto WorldToNdc(const Vector3& WorldPos, const Matrix& ViewMatrix, const Matrix& ProjMatrix) -> Vector3;
	auto NdcToWorld(const Vector3& NdcPos, const Matrix& ViewMatrix, const Matrix& ProjMatrix) -> Vector3;

	ComPtr<ID3D11Texture2D> RenderTex = nullptr;
	ComPtr<ID3D11RenderTargetView> RenderTexRTV = nullptr;

	ComPtr<ID3D11Texture2D> StagingTex = nullptr;

	ComPtr<ID3D11Device> Device = nullptr;
	ComPtr<ID3D11DeviceContext> DeviceContext = nullptr;

	ComPtr<ID3D11Texture2D> DepthStagingTex = nullptr;

	ComPtr<ID3D11Buffer> LookupCB;

	PixelShader* LookupShader = nullptr;

	class RenderingSystem* MyRenderingSystem;
};
