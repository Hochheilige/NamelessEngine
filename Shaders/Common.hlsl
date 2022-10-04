#ifndef __COMMON_HLSL__
#define __COMMON_HLSL__

#include "Lights.hlsl"

cbuffer CBPerDraw : register(b0)
{
	float4x4 WorldToView;
	float4x4 ViewToClip;
	float4x4 WorldToClip;
	float3 cameraWorldPos;
	float pad;
};

cbuffer CBLights : register(b1)
{
	LightData lightData;
};



cbuffer CBPerObject : register(b2)
{
	matrix ObjectToWorld;
	matrix NormalO2W;
	float4 Color;
	Material Mat;
};

#endif // __COMMON_HLSL__