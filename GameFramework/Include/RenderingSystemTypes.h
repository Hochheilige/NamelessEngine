#pragma once

#include "MathInclude.h"

#include <optional>

class PixelShader;

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
	std::optional<PixelShader*> OverridePixelShader;
};
