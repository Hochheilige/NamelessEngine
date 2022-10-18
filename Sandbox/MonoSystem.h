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

	//TODO check? throw error?
	MonoClass* FindClass(const char* name_space, const char* className) const;
	MonoMethodDesc* MakeMethodDescriptor(const char* descriptor, bool includeNamespace) const;
	MonoMethod* GetMethodByClass(MonoClass* clazz, MonoMethodDesc* desc) const;
	MonoObject* InvokeMethod(MonoMethod* method, void* obj, void** params, MonoObject** exc);
	MonoMethod* GetMethod(const char* nameSpace, const char* className, const char* desc);

	MonoDomain* rootDomain = nullptr;
	MonoDomain* appDomain = nullptr;
	MonoAssembly* m_ptrGameAssembly = nullptr;
	MonoImage* image = nullptr;

	/**
	 * Name of the main namespace in the 
	 */
	static constexpr char MAIN_NAMESPACE[] = "Scripts";
};
