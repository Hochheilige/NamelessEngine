#pragma once
#include <filesystem>
#include <map>
#include <xstring>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

class MonoSystem
{
protected:
	MonoSystem();
public:
	~MonoSystem();

	MonoImage* GetImage();
	
	void RestartMono();
	//TODO check? throw error?
	MonoClass* FindClass(const char* name_space, const char* className);
	MonoMethodDesc* MakeMethodDescriptor(const char* descriptor, bool includeNamespace);
	MonoMethod* GetMethod(MonoClass* clazz, MonoMethodDesc* desc);
	MonoMethod* GetMethod(MonoClass* clazz, const char* methodName, int paramCount);
	MonoMethod* GetMethod(const char* nameSpace, const char* className, const char* desc);
	MonoMethod* GetVirtualMethod(const char* earliestAncestorNamespace, const char* earliestAncestorClassName,
	                             const char* methodDesc, uint32_t objHandle);
	MonoObject* InvokeInstanceMethod(MonoMethod* method, uint32_t objHandle, void** params, MonoObject** exc);
	MonoObject* InvokeStaticMethod(MonoMethod* method, void** params, MonoObject** exception);
	uint32_t GCHandle(MonoObject* obj);
	MonoObject* CreateClassInstance(MonoClass* klass, bool initialize = true);
	MonoObject* GetHandledObject(uint32_t objHandle);
	void FreeHandledObject(uint32_t objHandle);
	static void PrintAssemblyTypes(MonoAssembly* assembly);
	static char* ReadFile(const std::filesystem::path& assemblyPath, uintmax_t& fileSize);
	static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath);

	static MonoSystem* GetInstance();

	int version = 0;
	MonoDomain* rootDomain = nullptr;
	MonoDomain* appDomain = nullptr;
	MonoAssembly* scriptAssembly = nullptr;
	MonoImage* image = nullptr;

	static MonoSystem* Instance;
	std::map<std::string, MonoMethodDesc*> monoDescsCache;
	/**
	 * Name of the main namespace in the 
	 */
	static constexpr char MAIN_NAMESPACE[] = "Scripts";
private:
	void InitializeMono();
};
