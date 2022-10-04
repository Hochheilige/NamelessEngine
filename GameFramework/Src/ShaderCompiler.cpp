#include "ShaderCompiler.h"

#include <d3dcompiler.h>

#include <wrl/client.h>

#include <iostream>

#include <locale>
#include <codecvt>

static const std::string ErrorMessagePrefix = "Shader Compiler Error:";

using namespace Microsoft::WRL;

bool ShaderCompiler::Compile(ID3DBlob** OutShaderByteCode)
{
	if (PathToShader.empty())
	{
		std::cout << ErrorMessagePrefix << "Path to shader has not been set" << std::endl;
		return false;
	}

	if (EntryPoint.empty())
	{
		std::cout << ErrorMessagePrefix << "Entry point has not been set" << std::endl;
		return false;
	}

	if (Target.empty())
	{
		std::cout << ErrorMessagePrefix << "Target has not been set" << std::endl;
		return false;
	}

	ComPtr<ID3DBlob> errors = nullptr;

	const UINT flags1 = IsDebug ? D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION : 0;

	HRESULT res = D3DCompileFromFile(PathToShader.c_str(),
		Macros.size() > 1 ? (&Macros[0]) : nullptr /*macros*/,
		D3D_COMPILE_STANDARD_FILE_INCLUDE  /*inlcude*/,
		EntryPoint.c_str(),
		Target.c_str(),
		flags1,
		0,
		OutShaderByteCode,
		errors.GetAddressOf());

	if (FAILED(res))
	{
		if (errors)
		{
			std::cout << "Failed to compile shader with the following macros: ";
			for (auto macro : Macros)
			{
				if (macro.Name != nullptr)
				{
					std::cout << macro.Name << ": " << macro.Definition << ";";
				}
			}
			char* compileErrors = (char*)(errors->GetBufferPointer());

			std::cout << ErrorMessagePrefix << compileErrors << std::endl;
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			//std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			std::wcout /*<< converter.from_bytes(ErrorMessagePrefix)*/ << L"Could not locate file by path: " << PathToShader.c_str() << std::endl;
		}
		return false;
	}

    return true;
}

void ShaderCompiler::SetPathToShader(const std::wstring& Path)
{
	PathToShader = Path;
}

void ShaderCompiler::SetIsDebug(const bool InIsDebug)
{
	IsDebug = InIsDebug;
}

void ShaderCompiler::SetEntryPoint(const std::string& InEntryPoint)
{
	EntryPoint = InEntryPoint;
}

void ShaderCompiler::SetTarget(const std::string& InTarget)
{
	Target = InTarget;
}

void ShaderCompiler::AddMacro(const D3D_SHADER_MACRO& Macro)
{
	Macros.pop_back();
	Macros.push_back(Macro);
	Macros.push_back({ nullptr, nullptr });
}

void ShaderCompiler::ClearMacros()
{
	Macros.clear();
	Macros.push_back({ nullptr, nullptr });
}
