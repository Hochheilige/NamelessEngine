#pragma once

#include <string>
#include <d3dcommon.h>
#include <vector>

#include "Shader.h"

class ShaderCompiler
{

public:
	ShaderCompiler() = default;

	bool Compile(ID3DBlob** OutResult);

	template<class T>
	T* CreateShader(bool Initialize = true)
	{
		T* shader = new T();

		/*if (!Compile(shader->GetByteCodeRef().GetAddressOf()))
		{
			delete shader;
			return nullptr;
		}*/

		ComPtr<ID3DBlob> byteCode;

		for (int i = 0; i < static_cast<int>(ShaderFlag::MAX); ++i)
		{
			ClearMacros();
			for (int j = 1; j <= i; j<<=1)
			{
				if (j & i)
				{
					ShaderFlag flags = static_cast<ShaderFlag>(j);
					AddMacro({ GetFlagString(flags), "1" });
				}
				
			}
			Compile(&byteCode);
			shader->Initialize(byteCode.Get(), static_cast<ShaderFlag>(i));
		}

		
		return shader;
	}

	void SetPathToShader(const std::wstring& Path);

	void SetIsDebug(const bool InIsDebug);

	void SetEntryPoint(const std::string& InEntryPoint);

	void SetTarget(const std::string& InTarget);

	void AddMacro(const D3D_SHADER_MACRO& Macro);

	void ClearMacros();

private:

	std::wstring PathToShader;

	bool IsDebug = false;

	std::string EntryPoint;

	std::string Target;

	std::vector<D3D_SHADER_MACRO> Macros;
	
};

