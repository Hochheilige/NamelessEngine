#include "MonoSystem.h"
#include "Mappings.h"
#include <filesystem>

MonoSystem* MonoSystem::Instance = nullptr;

//TODO move to GameFramework
MonoSystem::MonoSystem()
{
	auto exePath = __argv[0];
	std::filesystem::path path(exePath);
	auto parentPath = path.parent_path().string();
	auto result = parentPath.c_str();

	mono_set_assemblies_path((parentPath + "/mono/lib/4.5").c_str());

	//TODO add exceptions/message if domain/assembly/image can't be loaded

	rootDomain = mono_jit_init("NamelessEngine");

	if (rootDomain) {
		auto pathStr = parentPath + "\\Scripts.dll";

		appDomain = mono_domain_create_appdomain(const_cast<char*>("EngineAppDomain"), nullptr);
		mono_domain_set(appDomain, true);

		scriptAssembly = mono_domain_assembly_open(appDomain, pathStr.c_str());
		if (scriptAssembly) {
			PrintAssemblyTypes(scriptAssembly);
			image = mono_assembly_get_image(scriptAssembly);
			if (image) {
				/*mono_add_internal_call("Scripts.Script::CreateCubeObject", &Mappings::CS_CreateObj);
				mono_add_internal_call("Scripts.AudioComponent::InternalOnRegister", &Mappings::CS_AudioOnCreate);*/
			}
		}
	}
}


MonoImage* MonoSystem::GetImage()
{
	return image;
}

MonoClass* MonoSystem::FindClass(const char* nameSpace, const char* className)
{
	return mono_class_from_name(image, nameSpace, className);
}

MonoMethodDesc* MonoSystem::MakeMethodDescriptor(const char* descriptor, bool includeNamespace)
{
	if (monoDescsCache.contains(descriptor)) {
		return monoDescsCache.find(descriptor)->second;
	}

	auto methodDesc = mono_method_desc_new(descriptor, includeNamespace);
	monoDescsCache.emplace(descriptor, methodDesc);

	return methodDesc;
}

MonoMethod* MonoSystem::GetMethod(MonoClass* clazz, MonoMethodDesc* desc)
{
	return mono_method_desc_search_in_class(desc, clazz);
}

MonoMethod* MonoSystem::GetMethod(MonoClass* clazz, const char* methodName, int paramCount)
{
	return mono_class_get_method_from_name(clazz, methodName, paramCount);
}

MonoMethod* MonoSystem::GetMethod(const char* nameSpace, const char* className, const char* desc)
{
	const auto clazz = FindClass(nameSpace, className);

	auto fullDesc = std::string(nameSpace);
	fullDesc.append(".");
	fullDesc.append(className);
	fullDesc.append(":");
	fullDesc.append(desc);

	const auto desciptor = MakeMethodDescriptor(fullDesc.c_str(), true);
	return GetMethod(clazz, desciptor);
}

MonoObject* MonoSystem::CreateClassInstance(MonoClass* klass)
{
	MonoObject* classInstance = mono_object_new(appDomain, klass);
	mono_runtime_object_init(classInstance);
	return classInstance;
}

MonoMethod* MonoSystem::GetVirtualMethod(const char* earliestAncestorNamespace, const char* earliestAncestorClassName, const char* methodDesc, MonoObject* obj)
{
	auto baseMethod = GetMethod(earliestAncestorNamespace, earliestAncestorClassName, methodDesc);
	return mono_object_get_virtual_method(obj, baseMethod);
}

MonoObject* MonoSystem::InvokeMethod(MonoMethod* method, void* obj, void** params,
	MonoObject** exception)
{
	return mono_runtime_invoke(method, obj, params, exception);
}

void MonoSystem::PrintAssemblyTypes(MonoAssembly* assembly) {
	MonoImage* image = mono_assembly_get_image(assembly);

	// Get Metadata table of type definitions
	// All tables definitions - http://docs.go-mono.com/?link=xhtml%3adeploy%2fmono-api-metadata.html
	const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
	int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

	for (int32_t i = 0; i < numTypes; i++) {
		// Get current row with MONO_TYPEDEF_SIZE columns number
		uint32_t cols[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

		// Decode data
		const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
		const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

		printf("%s.%s\n", nameSpace, name);
	}
}

MonoSystem* MonoSystem::GetInstance()
{
	if(Instance == nullptr)
	{
		Instance = new MonoSystem;
	}

	return Instance;
}
