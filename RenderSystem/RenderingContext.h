#pragma once

#include "Scene.h"
#include "Renderer.h"

class RenderingContext
{
public:

	RenderingContext();

	void Initialize(Scene* current_scene);

	void DrawRenderers();

private:
	Scene* scene;

	ComPtr<ID3D11Buffer> PerDrawCB;
	ComPtr<ID3D11SamplerState> DefaultSampler;

	ComPtr<ID3D11DepthStencilState> OpaqueDepthStencilState;

	ComPtr<ID3D11RasterizerState> CullBackRasterizerState;
	ComPtr<ID3D11RasterizerState> CullFrontRasterizerState;

	ComPtr<ID3D11BlendState> OpaqueBlendState;

	void SetScreenSizeViewport();
};
