#pragma once

#include <wrl/client.h>
using namespace Microsoft::WRL;

#include <memory>

#include "MathInclude.h"
#include "GBuffer.h"
#include "RenderingSystemTypes.h"


class Actor;
class Camera;
class Game;
class LightBase;
class ObjectLookupHelper;
class PixelShader;
class Renderer;
class DebugDrawer;

class RenderingSystem
{
	friend ObjectLookupHelper;
public:

	RenderingSystem(class Game* InGame);

	void RegisterRenderer(Renderer* InRenderer);
	void UnregisterRenderer(Renderer* InRenderer);

	void RegisterLight(LightBase* Light);
	void UnregisterLight(LightBase* Light);

	// @TODO: Remove DeltaTime?
	void Draw(float DeltaTime, const Camera* InCamera);

	void* GetViewportTextureID() { return ViewportSRV.Get(); }

	// TODO: add a delegate and subscribe to resize event
	void HandleScreenResize(const Vector2& NewSize);

	auto GetActorUnderPosition(const Vector2& Pos)->Actor*;

	auto GetWorldPositionUnerScreenPosition(const Vector2& Pos)->Vector3;

	auto GetDebugDrawer() const -> DebugDrawer* { return debugDrawer.get(); }

private:

	void PerformShadowmapPass();
	// @TODO: should create Forward and Deferred RenderingSystemState objects that would implement State Pattern (strategy?)
	void PerformForwardOpaquePass();

	void PerformOpaquePass(float DeltaTime);
	void PerformLightingPass(float DeltaTime);

	void PerformDebugPass();

	void ResizeViewport(int Width, int Height);

private:

	Game* MyGame;

	GBuffer GeometryBuffer;

	ComPtr<ID3D11Buffer> PerDrawCB;
	ComPtr<ID3D11Buffer> PerObjectCB;
	ComPtr<ID3D11Buffer> LightsCB;
	
	ComPtr<ID3D11SamplerState> DefaultSampler;
	ComPtr<ID3D11SamplerState> ShadowmapSampler;

	ComPtr<ID3D11DepthStencilState> OpaqueDepthStencilState;
	ComPtr<ID3D11DepthStencilState> LightingLessDepthStencilState;
	ComPtr<ID3D11DepthStencilState> LightingGreaterDepthStencilState;
	ComPtr<ID3D11DepthStencilState> DisabledDepthStencilState;

	

	CBPerDraw PerDrawData = CBPerDraw{};
	
	ComPtr<ID3D11RasterizerState> CullBackRasterizerState;
	ComPtr<ID3D11RasterizerState> CullFrontRasterizerState;
	ComPtr<ID3D11RasterizerState> CullNoneRasterizerState;

	ComPtr<ID3D11BlendState> OpaqueBlendState;
	ComPtr<ID3D11BlendState> LightBlendState;

	// @TODO: Use smart pointers for this?
	std::vector<Renderer*> Renderers;

	std::vector<LightBase*> Lights;

	Color DiffuseClearColor = Color(0.15f, 0.1f, 0.3f, 1.0f);

	ComPtr<ID3D11Texture2D> ViewportTex = nullptr;
	ComPtr<ID3D11ShaderResourceView> ViewportSRV = nullptr;
	ComPtr<ID3D11RenderTargetView> ViewportRTV = nullptr;

	ComPtr<ID3D11Texture2D> DepthStencilTex = nullptr;
	ComPtr<ID3D11DepthStencilView> DepthStencilView = nullptr;

	Vector2 ViewportSize;

	ObjectLookupHelper* MyObjectLookupHelper = nullptr;

	std::unique_ptr<DebugDrawer> debugDrawer;
private:

	void SetScreenSizeViewport();
};