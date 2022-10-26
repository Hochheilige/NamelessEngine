#pragma once

#include <wrl/client.h>

#include "MathInclude.h"
#include "GBuffer.h"

#pragma pack(push, 4)
struct LightData
{
	Matrix WorldToLightClip;
	Vector4 Position;
	Vector3 Direction;
	float pad;
	// @TODO: replace with proper 4 floats
	float Intensity = 1.0f;
	Vector3 Params;
	Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
};
#pragma pack(pop)

using namespace Microsoft::WRL;

class Game;
class Renderer;
class LightBase;
class Camera;

struct ID3D11Buffer;
struct ID3D11SamplerState;
struct ID3D11DepthStencilState;

// @TODO: move this to rendering system?
#pragma pack(push, 4)
struct CBPerDraw
{
	Matrix WorldToView;
	Matrix ViewToClip;
	Matrix WorldToClip;
	Vector3 CameraWorldPos;
	float pad;
};

struct LitMaterial
{
	float ambientCoef = 0.1f;
	float specularCoef = 0.5f;
	float specularExponent = 1.0f;
	float diffuesCoef = 0.8f;
};

struct CBPerObject
{
	Matrix ObjectToWorld;
	Matrix NormalObjectToWorld;
	Color Color;
	LitMaterial Mat;
};

struct CBLights
{
	LightData LightData;
};
#pragma pack(pop)

// @TODO: create rendering system context and pass it to mesh renderer
// RenderingSystemContext should contain:
// Camera to use
// Scene to render?
// Shaders override with an equivalent of TOptional
// Different flags that would tell us:
//  rendering type (deferred or forward) - we'll need to use them to select the right shader type
struct RenderingSystemContext
{
	int ShaderFlags = 0;
};


class RenderingSystem
{

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


private:

	void PerformShadowmapPass();
	// @TODO: should create Forward and Deferred RenderingSystemState objects that would implement State Pattern (strategy?)
	void PerformForwardOpaquePass();

	void PerformOpaquePass(float DeltaTime);
	void PerformLightingPass(float DeltaTime);

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

	Color DiffuseClearColor = Color(0.0f, 0.4f, 0.0f, 1.0f);

	ComPtr<ID3D11Texture2D> ViewportTex = nullptr;
	ComPtr<ID3D11ShaderResourceView> ViewportSRV = nullptr;
	ComPtr<ID3D11RenderTargetView> ViewportRTV = nullptr;

	ComPtr<ID3D11Texture2D> DepthStencilTex = nullptr;
	ComPtr<ID3D11DepthStencilView> DepthStencilView = nullptr;



private:

	void SetScreenSizeViewport();
};