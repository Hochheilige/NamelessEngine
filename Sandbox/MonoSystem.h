#pragma once
#include <xstring>
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
	MonoMethod* GetMethod(MonoClass* clazz, MonoMethodDesc* desc) const;
	MonoMethod* GetMethod(MonoClass* clazz, const char* methodName, int paramCount) const;
	MonoMethod* GetMethod(const char* nameSpace, const char* className, const char* desc) const;
	MonoMethod* GetVirtualMethod(const char* earliestAncestorNamespace, const char* earliestAncestorClassName,
	                             const char* methodDesc, MonoObject* obj) const;
	MonoObject* InvokeMethod(MonoMethod* method, void* obj, void** params, MonoObject** exc);
	void PrintAssemblyTypes(MonoAssembly* assembly);

	MonoDomain* rootDomain = nullptr;
	MonoDomain* appDomain = nullptr;
	MonoAssembly* scriptAssembly = nullptr;
	MonoImage* image = nullptr;

	/**
	 * Name of the main namespace in the 
	 */
	static constexpr char MAIN_NAMESPACE[] = "Scripts";
};
