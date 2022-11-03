struct VS_IN
{
	float3 pos : POSITION0;
#if !defined(DEFERRED_LIGHTING)
	float3 normal : NORMAL0;
	float3 binormal : BINORMAL0;
	float3 tangent : TANGENT0;
	float2 uv : TEXCOORD0;
#endif
};

struct PS_IN
{
	float4 pos : SV_POSITION;
#if !defined(DEFERRED_LIGHTING)
	float3 normal : NORMAL0;
	float4 binormal : BINORMAL;
	float4 tangent : TANGENT;
	float2 uv : TEXCOORD0;
	float3 worldPos : POSITION0;
#endif
};

#include "Common.hlsl"
#include "Lights.hlsl"

//////////////////////////////////////////////////////
// Shader code
//////////////////////////////////////////////////////

PS_IN VSMain(
#if defined(QUAD_ONLY)
	uint id: SV_VertexID
#else
	VS_IN input
#endif
)
{
	PS_IN output = (PS_IN)0;

#if !defined(DEFERRED_LIGHTING) & !defined(QUAD_ONLY)
	matrix objectToClip = mul(ObjectToWorld, WorldToClip);
	output.pos = mul(float4(input.pos, 1.0f), objectToClip);
	output.uv = input.uv;
	output.normal = mul(input.normal, NormalO2W);
	output.binormal = normalize(mul(float4(input.binormal.xyz, 0.0f), NormalO2W));
	output.tangent = normalize(mul(float4(input.tangent.xyz, 0.0f), NormalO2W));
	output.worldPos = mul(float4(input.pos, 1.0f), ObjectToWorld).xyz;
#elif defined(QUAD_ONLY)
	float2 inds = float2(id & 1, (id & 2) >> 1);
	output.pos = float4(inds * float2(2, -2) + float2(-1, 1), 0, 1);
#else
	matrix objectToClip = mul(ObjectToWorld, WorldToClip);
	output.pos = mul(float4(input.pos.xyz, 1.0f), objectToClip);
#endif

	return output;
}

#if defined(FORWARD_RENDERING) | defined(DEFERRED_OPAQUE) | defined(DEFERRED_LIGHTING)
Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t2);
#if !defined(DEFERRED_LIGHTING)
Texture2D SpecularMap : register(t3);
#else
Texture2D WorldPosMap : register(t3);
#endif
#endif

SamplerState DefaultSampler : register(s0);

// @TODO: only if shadowmap is defined?
Texture2D shadowMap : register(t1);
SamplerComparisonState shadowSampler : register(s1);

struct PSOutput
{
	float4 diffuse : SV_Target0;
#if defined(DEFERRED_OPAQUE) & !defined(DEFERRED_LIGHTING)
	float4 normal : SV_Target1;
	float4 worldPos : SV_Target2;
#endif
};

PSOutput PSMain(PS_IN input) : SV_Target
{
	PSOutput ret = (PSOutput)0;
#if !defined(DEFERRED_LIGHTING)
#if defined(FORWARD_RENDERING) | defined(DEFERRED_OPAQUE)
	float4 col = DiffuseMap.Sample(DefaultSampler, input.uv) * Color;
	float specular = SpecularMap.Sample(DefaultSampler, input.uv.xy).r;
	float3 normal = NormalMap.Sample(DefaultSampler, input.uv.xy).xyz;

	float3 pixelPos = input.worldPos;
#else
	float4 col = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float specular = 0.5f;
	float3 normal = float3(0.5f, 0.5f, 1.0f);
	float3 pixelPos = input.worldPos;
#endif
#endif


#if !defined(DEFERRED_LIGHTING)
	// todo: reorthogonize normal?
	float3 T = normalize(input.tangent.xyz);
	float3 B = normalize(input.binormal.xyz);
	float3 N = normalize(input.normal.xyz);
	float3x3 TBN = float3x3(T, B, N);

	float3 unpackedNormal = normalize(normal * 2.0f - 1.0f);
	// This is here because the normal maps I have have a non-inverted y (which is okay for OpenGL, but not for DirectX)
	unpackedNormal.g = -1.0f * unpackedNormal.g;

	unpackedNormal = mul(unpackedNormal, TBN);
#else
	float3 unpackedNormal = NormalMap.Load(float3(input.pos.xy, 0)).xyz;
	float4 col = DiffuseMap.Load(float3(input.pos.xy, 0));
	float3 pixelPos = WorldPosMap.Load(float3(input.pos.xy, 0)).xyz;
	float specular = col.a;
#endif

#if defined(FORWARD_RENDERING) | defined(DEFERRED_LIGHTING)
#if defined (DIRECTIONAL_LIGHT)
	float4 lightSpacePos = mul(float4(pixelPos, 1.0f), lightData.WorldToLightClip);
	lightSpacePos = lightSpacePos / lightSpacePos.w;
	float2 smUV = lightSpacePos.xy;
	smUV = smUV * 0.5f + 0.5f;
	smUV.y = 1.0f - smUV.y;

	float shadow = shadowMap.SampleCmp(shadowSampler, smUV, lightSpacePos.z - 0.003f);

	ret.diffuse.xyz = shadow * CalculateDirLight(pixelPos, cameraWorldPos, unpackedNormal, col.xyz, specular, lightData, Mat);
	ret.diffuse.a = 1.0f;
#elif defined(AMBIENT_LIGHT)
	ret.diffuse = float4(col.rgb * lightData.intensity * lightData.color.rgb, 1.0f);
#elif defined(POINT_LIGHT)
	ret.diffuse = float4(CalculatePointLight(pixelPos, cameraWorldPos, unpackedNormal, col.xyz, lightData, Mat), 1.0f);
#endif // defined (DIRECTIONAL_LIGHT)
#endif

#if defined(DEFERRED_OPAQUE) & !defined(DEFERRED_LIGHTING)
	ret.worldPos = float4(pixelPos, 1.0f);
	ret.diffuse.xyz = col.xyz;
	ret.diffuse.a = specular;
	ret.normal = float4(unpackedNormal, 0.0f);
	//ret.diffuse.xyz = normalize(ret.normal.xyz) * 0.5f + 0.5f;
#endif


	//col.xyz = shadow.xxx;
	return ret;
}