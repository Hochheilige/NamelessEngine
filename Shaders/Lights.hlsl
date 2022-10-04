#ifndef __LIGHTS_HLSL__
#define __LIGHTS_HLSL__

#include "Common.hlsl"

struct LightData
{
	float4x4 WorldToLightClip;
	float4 position;
	float3 direction;
	float pad;
	float intensity;
	float3 params;
	float4 color;
};

struct Material
{
	float ambientCoef;
	float specularCoef;
	float specularExponent;
	float diffuseCoef;
};

float3 CalculateDirLight(
	float3 pixelPos,
	float3 viewPos,
	float3 normal,
	float3 baseColor,
	float spec,
	LightData light, 
	Material mat
)
{
	float3 viewDir = normalize(viewPos - pixelPos);
	//@TODO: don't use mat.specularCoef here?
	float3 lightDiffuse =  0.1f * saturate(-dot(light.direction, normal)) * baseColor;
	float3 lightSpecular = mat.specularCoef * pow(saturate(dot(reflect(light.direction, normal), viewDir)), 50.0f);

	return (lightDiffuse + lightSpecular) * light.intensity * light.color;
}

float3 CalculatePointLight
(
	float3 pixelPos,
	float3 viewPos,
	float3 normal,
	float3 baseColor,
	LightData light,
	Material mat
)
{
	float3 viewDir = normalize(viewPos - pixelPos);
	// attenuation
	float distance = length(light.position - pixelPos);
	// x - constant
	// y - linear
	// z - quadratic
	float attenuation = 1.0f / (light.params.x + light.params.y * distance + light.params.z * distance * distance);
	// diffuse shading
	float3 lightDir = normalize(light.position - pixelPos);
	float3 diffuse = saturate(dot(normal, lightDir)) * baseColor;
	// spucular shading
	float3 reflectDir = reflect(-lightDir, normal);
	float3 specular = pow(saturate(dot(viewDir, reflectDir)), mat.specularExponent) * mat.specularCoef;

	float3 lightCol = light.color * light.intensity;
	return (diffuse + specular) * lightCol * attenuation;
}

float3 CalculateSpotLight
(
	float3 pixelPos,
	float3 viewPos,
	float3 normal,
	float3 baseColor,
	LightData light,
	Material mat
)
{
	float3 viewDir = normalize(viewPos - pixelPos);
	float3 lightDir = normalize(pixelPos - light.position);
	float3 spotDir = normalize(light.direction);
	float theta = dot(spotDir, lightDir);

	// x - cutoff
	// y - outer cutoff
	float phi = cos(light.params.x);
	float gamma = cos(light.params.y);


	float alpha = clamp((theta - gamma) / (phi - gamma), 0.0f, 1.0f);


	float3 diffuse = saturate(dot(normal, -lightDir)) * baseColor;
	float3 reflectDir = reflect(lightDir, normal);
	float3 specular = pow(saturate(dot(viewDir, reflectDir)), mat.specularExponent) * mat.specularCoef;


	float3 lightColor = light.color * light.intensity;
	return (diffuse + specular) * lightColor * alpha;
}

float3 CalculateAmbientLight()
{

}
#endif // __LIGHTS_HLSL__
