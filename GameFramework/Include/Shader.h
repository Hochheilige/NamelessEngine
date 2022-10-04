#pragma once

#include <d3dcommon.h>
#include <d3d11.h>

#include <unordered_map>

#include <wrl/client.h>

using namespace Microsoft::WRL;

// @TODO: use int in shader compiler and all the methods to make the system more robust and allow different shaders to use different flags
// @TODO: avoid compiling every variation
enum class ShaderFlag
{
	None = 0,
	ForwardRendering = 1 << 0,
	DeferredOpaque = 1 << 1,
	DirectionalLight = 1 << 2,
	DeferredLighting = 1 << 3,
	QuadOnly = 1 << 4,
	AmbientLight = 1 << 5,
	PointLight = 1 << 6,
	MAX = 1 << 7
};

DEFINE_ENUM_FLAG_OPERATORS(ShaderFlag)

inline bool operator < (ShaderFlag a, ShaderFlag b) { return static_cast<int>(a) < static_cast<int>(b); }

extern LPCSTR MacroNames[7];

LPCSTR GetFlagString(ShaderFlag Flags);

class Shader
{
public:

	virtual void Initialize(ID3DBlob* ByteCode, ShaderFlag Flags = ShaderFlag::None) = 0;

	virtual void UseShader(ShaderFlag Flags = ShaderFlag::None) = 0;

protected:


	friend class ShaderCompiler;
};

class VertexShader : public Shader
{
public:
	//ComPtr<ID3D11VertexShader> GetD3DShaderPointer();

	virtual void Initialize(ID3DBlob* ByteCode, ShaderFlag Flags) override;

	virtual void UseShader(ShaderFlag Flags = ShaderFlag::None) override;

protected:
	std::unordered_map<ShaderFlag, ComPtr<ID3D11VertexShader>> ShaderVariations;

	ComPtr<ID3D11InputLayout> InputLayout;
};

class PixelShader : public Shader
{
public:
	//ComPtr<ID3D11PixelShader> GetD3DShaderPointer();

	virtual void Initialize(ID3DBlob* ByteCode, ShaderFlag Flags) override;

	virtual void UseShader(ShaderFlag Flags = ShaderFlag::None) override;

protected:
	std::unordered_map<ShaderFlag, ComPtr<ID3D11PixelShader>> ShaderVariations;
};

class BasicVertexShader : public VertexShader
{
public:
	virtual void Initialize(ID3DBlob* ByteCode, ShaderFlag Flags) override;
};

// todo: move to a separate file
class SimpleVertexShader : public VertexShader
{
public:
	virtual void Initialize(ID3DBlob* ByteCode, ShaderFlag Flags) override;
};

class TexturedVertexShader : public VertexShader
{
public:
	virtual void Initialize(ID3DBlob* ByteCode, ShaderFlag Flags) override;
	virtual void UseShader(ShaderFlag Flags = ShaderFlag::None) override;

protected:
	ComPtr<ID3D11InputLayout> PositionOnlyInputLayout;

};