#pragma once
#include <filesystem>
#include <map>
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
	MonoClass* FindClass(const char* name_space, const char* className);
	MonoMethodDesc* MakeMethodDescriptor(const char* descriptor, bool includeNamespace);
	MonoMethod* GetMethod(MonoClass* clazz, MonoMethodDesc* desc);
	MonoMethod* GetMethod(MonoClass* clazz, const char* methodName, int paramCount);
	MonoMethod* GetMethod(const char* nameSpace, const char* className, const char* desc);
	MonoMethod* GetVirtualMethod(const char* earliestAncestorNamespace, const char* earliestAncestorClassName,
	                             const char* methodDesc, MonoObject* obj);
	MonoObject* InvokeMethod(MonoMethod* method, void* obj, void** params, MonoObject** exc);
	MonoObject* CreateClassInstance(MonoClass* klass);
	static void PrintAssemblyTypes(MonoAssembly* assembly);
	char* ReadFile(const std::filesystem::path& assemblyPath, uintmax_t& fileSize) const;

	MonoDomain* rootDomain = nullptr;
	MonoDomain* appDomain = nullptr;
	MonoAssembly* scriptAssembly = nullptr;
	MonoImage* image = nullptr;

	std::map<std::string, MonoMethodDesc*> monoDescsCache;
	/**
	 * Name of the main namespace in the 
	 */
	static constexpr char MAIN_NAMESPACE[] = "Scripts";
};
