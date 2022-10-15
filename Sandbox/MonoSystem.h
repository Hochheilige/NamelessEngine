#pragma once
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

class MonoSystem
{
public:
	MonoSystem();
	~MonoSystem();

	MonoImage* GetImage();

	MonoDomain* m_ptrMonoDomain = nullptr;
	MonoAssembly* m_ptrGameAssembly = nullptr;
	MonoImage* m_ptrGameAssemblyImage = nullptr;
};

