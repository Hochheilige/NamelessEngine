struct VS_IN
{
	float3 pos : POSITION0;
	float4 col : COLOR0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
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

	//matrix objectToClip = mul(WorldToClip, ObjectToWorld);
	matrix objectToClip = mul(ObjectToWorld, WorldToClip);
	//output.pos = mul(mul(input.pos, ObjectToWorld), WorldToClip);
	//output.pos = mul(mul(input.pos, WorldToClip),ObjectToWorld);
	output.pos = mul(float4(input.pos, 1.0f), objectToClip);
	//output.pos = mul(objectToClip, input.pos);

	//output.pos = input.pos;
	output.col = float4(input.col.xyz, 1.0f);

	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
	float4 col = input.col;

	return col;
}

float4 PSPlainColor(PS_IN input) : SV_Target
{
	float4 col = Color;

	return col;
}