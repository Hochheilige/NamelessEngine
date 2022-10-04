struct VS_IN
{
	float3 pos : POSITION0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
};

//////////////////////////////////////////////////////
// Constant buffers
//////////////////////////////////////////////////////

#include "Common.hlsl"

//////////////////////////////////////////////////////
// Shader code
//////////////////////////////////////////////////////

PS_IN VSMain(VS_IN input)
{
	PS_IN output = (PS_IN)0;

	matrix objectToClip = mul(ObjectToWorld, WorldToClip);
	output.pos = mul(float4(input.pos, 1.0f), objectToClip);

	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
	float4 col = Color;

	return col;
}